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

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BaseJumpPower = 600;

	// References to owner and its components
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComp;

	UPROPERTY()
	class USkeletalMeshComponent* Mesh;

	// Reference to the push montage (set this from the editor)
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* PushMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	UAnimMontage* DoubleJumpMontage;

	
private:

	// Helper functions
	void TryPlayPushMontage(float SpeedThreshold);
	void ResetJump();

	// Timer handle for jump cooldown
	FTimerHandle JumpCooldownTimerHandle;
		
	float AppliedFrictionRate;
	bool bCanJump;
};
