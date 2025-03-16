#include "Components/SkateboardMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Core/SkateboarderCharacter.h"
#include "Engine/Engine.h"

USkateboardMovementComponent::USkateboardMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AppliedFrictionRate = DefaultFrictionRate;
	bCanJump = true;
}

void USkateboardMovementComponent::Initialize(ACharacter* InOwner)
{
	OwnerCharacter = InOwner;
	if (OwnerCharacter)
	{
		MovementComp = OwnerCharacter->GetCharacterMovement();
		Mesh = OwnerCharacter->GetMesh();
	}
}

void USkateboardMovementComponent::UpdateMovement(float DeltaTime)
{
	if (MovementComp && MovementComp->MaxWalkSpeed > 0.f)
	{
		OwnerCharacter->AddMovementInput(OwnerCharacter->GetActorForwardVector(), 1);

		float NewSpeed = FMath::FInterpTo(MovementComp->MaxWalkSpeed, 0.f, DeltaTime, AppliedFrictionRate);
		MovementComp->MaxWalkSpeed = NewSpeed;

		// Print the current speed on screen (debug)
		if (GEngine)
		{
			FString SpeedMessage = FString::Printf(TEXT("Current Speed: %.2f"), NewSpeed);
			GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Cyan, SpeedMessage);
		}
	}
}

void USkateboardMovementComponent::HandleMoveInput(float MoveValue)
{
	if (!MovementComp || MovementComp->IsFalling())
	{
		return;
	}

	if (MoveValue < 0.f)
	{
		AppliedFrictionRate = DefaultFrictionRate * BrakePower;
	}
	else if (MoveValue > 0.f)
	{
		AppliedFrictionRate = DefaultFrictionRate;
		TryPlayPushMontage(NormalSpeedThreshold);
	}
}

void USkateboardMovementComponent::HandleLeanInput(float LeanValue)
{
	if (!OwnerCharacter)
	{
		return;
	}
	if (!FMath::IsNearlyZero(LeanValue, 0.01f))
	{
		OwnerCharacter->AddControllerYawInput(LeanValue * LeanAngle * OwnerCharacter->GetWorld()->GetDeltaSeconds());
	}
}

void USkateboardMovementComponent::HandlePush()
{
	TryPlayPushMontage(BoostSpeedThreshold);
}

void USkateboardMovementComponent::HandleJump()
{
	if (!MovementComp || !OwnerCharacter || !Mesh || Mesh->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}
	
	if (MovementComp->IsFalling())
	{
		OnDoubleJumped.Broadcast();
		return;
	}

	if (bCanJump && MovementComp->MaxWalkSpeed > NormalSpeedThreshold)
	{
		float JumpPower = MovementComp->MaxWalkSpeed / 2.f;
		OwnerCharacter->LaunchCharacter(FVector(0, 0, JumpPower), false, true);
		OnJumpStart.Broadcast();
		bCanJump = false;
		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(JumpCooldownTimerHandle, this, &USkateboardMovementComponent::ResetJump, 1.0f, false);
	}
}

void USkateboardMovementComponent::ResetJump()
{
	bCanJump = true;
}

void USkateboardMovementComponent::ExecutePush()
{
	if (!MovementComp || MovementComp->IsFalling())
	{
		return;
	}

	if (MovementComp->MaxWalkSpeed < MaxSpeed)
	{
		FVector PushImpulse = OwnerCharacter->GetActorForwardVector() * PushForce;
		OwnerCharacter->LaunchCharacter(PushImpulse, true, false);

		if (MovementComp->MaxWalkSpeed < NormalSpeedThreshold)
		{
			MovementComp->MaxWalkSpeed = NormalSpeedThreshold * 2;
		}
		else
		{
			MovementComp->MaxWalkSpeed = MovementComp->MaxWalkSpeed + PushForce / 2;
		}
	}
}

void USkateboardMovementComponent::TryPlayPushMontage(float SpeedThreshold)
{
	if (!Mesh)
	{
		return;
	}

	if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
	{
		if (!AnimInstance->IsAnyMontagePlaying() && MovementComp->MaxWalkSpeed < SpeedThreshold && PushMontage)
		{
			AnimInstance->Montage_Play(PushMontage);
		}
	}
}
