#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h" // For FInputActionValue
#include "SkateboarderCharacter.generated.h"

UCLASS()
class SKATEBOARDER_API ASkateboarderCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASkateboarderCharacter();
	
	UFUNCTION()
	void ExecutePush();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDoubleJump();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Combined movement function for forward/backward input.
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	// Combined lean function for left/right input.
	UFUNCTION()
	void Lean(const FInputActionValue& Value);
private:
	// Push & Jump Functions
	UFUNCTION()
	void Push(const FInputActionValue& Value);

	UFUNCTION()
	void PerformJump();

	UFUNCTION()
	void ResetJump();

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
	float NormalSpeedThreshhold = 300;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BoostSpeedThreshhold = 600;
	

	// Jump Cooldown
	bool bCanJump = true;

	// Track if the player is moving forward (W pressed)
	bool bIsMovingForward = false;

	// Boost hold duration: the boost stays at high speed for this long before decaying.
	UPROPERTY(EditAnywhere, Category = "Movement")
	float BoostHoldDuration = 1.0f;

	// Time of the last push boost.
	float LastPushTime = 0.0f;

	// Enhanced Input Variables
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LeanAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* PushAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JumpAction;

	// Push Montage – assign a montage that contains a notify state (e.g., UAnimNotifyState_Push) to trigger ExecutePush().
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* PushMontage;

	// Timer Handles
	FTimerHandle PushDecayTimerHandle;
	FTimerHandle JumpCooldownTimerHandle;
	bool bBoosting;

	float AppliedFrictionRate = 1.0f;
};
