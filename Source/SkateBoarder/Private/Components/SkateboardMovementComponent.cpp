#include "Components/SkateboardMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Core/SkateboarderCharacter.h"
#include "Engine/Engine.h"

USkateboardMovementComponent::USkateboardMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void USkateboardMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateMovement(DeltaTime);
}

void USkateboardMovementComponent::UpdateMovement(float DeltaTime)
{
	if (MovementComp && MovementComp->MaxWalkSpeed > 0.f)
	{
		OwnerCharacter->AddMovementInput(OwnerCharacter->GetActorForwardVector(), 1);

		// Apply friction to gradually reduce speed
		float NewSpeed = FMath::FInterpTo(MovementComp->MaxWalkSpeed, 0.f, DeltaTime, AppliedFrictionRate);

		// If the character's velocity is nearly zero, force MaxWalkSpeed to 0
		if (FMath::IsNearlyZero(OwnerCharacter->GetVelocity().Size(), 100.0f))
		{
			NewSpeed = 0.f;
		}

		MovementComp->MaxWalkSpeed = NewSpeed;
		
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
	if (!MovementComp || !OwnerCharacter || !Mesh)
	{
		return;
	}
	
	if (MovementComp->IsFalling())
	{
		//perform an air spin
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DoubleJumpMontage);
			OnDoubleJumped.Broadcast();
		}
		return;
	}

	if (bCanJump && MovementComp->MaxWalkSpeed > NormalSpeedThreshold/2)
	{
		if (Mesh->GetAnimInstance()->IsAnyMontagePlaying())
		{
			Mesh->GetAnimInstance()->StopAllMontages(0.2f);
		}
		float JumpPower = BaseJumpPower + MovementComp->MaxWalkSpeed / 10.0f;
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
