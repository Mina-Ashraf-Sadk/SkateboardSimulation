#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/SkateboardMovementComponent.h" 
#include "SkateboarderCharacter.generated.h"


UCLASS()
class SKATEBOARDER_API ASkateboarderCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASkateboarderCharacter();
	
	UFUNCTION()
	void ExecutePush(); // Called (typically via animation notify)

protected:
	// Lifecycle Functions
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input Handling (delegated to movement component)
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Lean(const FInputActionValue& Value);

	UFUNCTION()
	void Push(const FInputActionValue& Value);
	
	UFUNCTION()
	void PerformJump();

private:
	// New Movement Component for decoupled movement logic
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkateboardMovementComponent* SkateMovementComponent;

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
};
