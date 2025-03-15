#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Character.h>
#include "SkateboarderCharacter.generated.h"

UCLASS()
class SKATEBOARDER_API ASkateboarderCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASkateboarderCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Movement Functions
	void MoveForward(float Value);
	void Turn(float Value);
	void Push();       // Speed Up
	void SlowDown();   // Apply Friction
	void PerformJump();

	// Helper Function for Movement
	void ApplyFriction(float DeltaTime);

	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//class UScoreComponent* ScoreComp;

	// Movement Variables
	UPROPERTY(EditAnywhere, Category = "Movement")
	float PushForce = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SlowDownRate = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 60.0f;
};
