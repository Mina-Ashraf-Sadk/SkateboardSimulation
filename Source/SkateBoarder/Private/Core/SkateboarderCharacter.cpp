#include "Core/SkateboarderCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"

ASkateboarderCharacter::ASkateboarderCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;

}


void ASkateboarderCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AppliedFrictionRate = DefaultFrictionRate;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputMappingContext)
				Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ASkateboarderCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetCharacterMovement()->MaxWalkSpeed > 0.f)
	{
		AddMovementInput(GetActorForwardVector(), 1);

		float NewSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, 0.f, DeltaTime, AppliedFrictionRate);
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

		// Print the current speed on screen
		FString SpeedMessage = FString::Printf(TEXT("Current Speed: %.2f"), NewSpeed);
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Cyan, SpeedMessage);
	}
}


void ASkateboarderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateboarderCharacter::Move);
		EnhancedInput->BindAction(LeanAction, ETriggerEvent::Triggered, this, &ASkateboarderCharacter::Lean);
		EnhancedInput->BindAction(PushAction, ETriggerEvent::Triggered, this, &ASkateboarderCharacter::Push);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkateboarderCharacter::PerformJump);
	}
}
void ASkateboarderCharacter::Move(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	bIsMovingForward = (MoveValue > 0.1f);

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (MoveValue < 0.0f)
	{
		AppliedFrictionRate = DefaultFrictionRate * BrakePower;
	}
	else if (MoveValue > 0.0f)
	{
		AppliedFrictionRate = DefaultFrictionRate;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->IsAnyMontagePlaying() && (GetCharacterMovement()->MaxWalkSpeed < NormalSpeedThreshhold))
		{
			AnimInstance->Montage_Play(PushMontage);
		}
	}
}

void ASkateboarderCharacter::Lean(const FInputActionValue& Value)
{
	float LeanValue = Value.Get<float>();
	if (!FMath::IsNearlyZero(LeanValue, 0.01f))
	{
		AddControllerYawInput(LeanValue * LeanAngle * GetWorld()->GetDeltaSeconds());
	}
}

void ASkateboarderCharacter::Push(const FInputActionValue& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->IsAnyMontagePlaying() && (GetCharacterMovement()->MaxWalkSpeed < BoostSpeedThreshhold))
	{
		AnimInstance->Montage_Play(PushMontage);
	}
}


void ASkateboarderCharacter::PerformJump()
{
	if (GetCharacterMovement()->IsFalling())
	{
		OnDoubleJump();
		return;
	}
	if (bCanJump && !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() && GetCharacterMovement()->MaxWalkSpeed > 300)
	{
		float JumpPower = GetCharacterMovement()->MaxWalkSpeed / 2.0f;
		LaunchCharacter(FVector(0, 0, JumpPower), false, true);
		bCanJump = false;
		GetWorld()->GetTimerManager().SetTimer(JumpCooldownTimerHandle, this, &ASkateboarderCharacter::ResetJump, 1.0f, false);
	}
}

void ASkateboarderCharacter::ResetJump()
{
	bCanJump = true;
}

void ASkateboarderCharacter::ExecutePush()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}
	if (GetCharacterMovement()->MaxWalkSpeed < MaxSpeed)
	{
		FVector PushImpulse = GetActorForwardVector() * PushForce;
		LaunchCharacter(PushImpulse, true, false);

		if (bBoosting)
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
		}
		else
		{
			if (GetCharacterMovement()->MaxWalkSpeed < NormalSpeedThreshhold)
			{
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeedThreshhold * 2;
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed + PushForce/2 ;
			}
			 
		}
	}
}