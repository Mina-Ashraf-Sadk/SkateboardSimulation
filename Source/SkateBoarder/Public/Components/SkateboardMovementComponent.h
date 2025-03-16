#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkateboardMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAction);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SKATEBOARDER_API USkateboardMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkateboardMovementComponent();

	// Initialize the component with the owning character
	void Initialize(class ACharacter* InOwner);

	// Called every tick from the character
	void UpdateMovement(float DeltaTime);

	// Input handling functions
	void HandleMoveInput(float MoveValue);
	void HandleLeanInput(float LeanValue);
	void HandlePush();
	void HandleJump();

	// Called (typically from an animation notify) to apply push logic
	void ExecutePush();

	UPROPERTY(BlueprintAssignable)
	FOnAction OnJumpStart;

	UPROPERTY(BlueprintAssignable)
	FOnAction OnDoubleJumped;

	UPROPERTY(BlueprintAssignable)
	FOnAction OnSkatePushed;

protected:
	// Movement Variables
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float PushForce = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DefaultFrictionRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BrakePower = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float LeanAngle = 30.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float NormalSpeedThreshold = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BoostSpeedThreshold = 600.f;

	// Boost hold duration: the boost stays at high speed for this long before decaying.
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BoostHoldDuration = 1.0f;

	// Internal state
	float AppliedFrictionRate;
	bool bCanJump;

	// References to owner and its components
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComp;

	UPROPERTY()
	class USkeletalMeshComponent* Mesh;

	// Timer handle for jump cooldown
	FTimerHandle JumpCooldownTimerHandle;

	// Reference to the push montage (set this from the editor)
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* PushMontage;

	// Helper functions
	void TryPlayPushMontage(float SpeedThreshold);
	void ResetJump();
};
