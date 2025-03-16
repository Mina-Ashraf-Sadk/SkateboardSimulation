#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdated, int32, NewScore);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SKATEBOARDER_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScoreComponent();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const { return PlayerScore; }

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreUpdated OnScoreUpdated;

protected:
	virtual void BeginPlay() override;

private:
	int32 PlayerScore;
	float JumpStartTime;
	int32 StyleScore;
	float ObstacleBonus;
	FTimerHandle LandingCheckTimerHandle;

	UPROPERTY()
	class ASkateboarderCharacter* PlayerCharacter;

	UPROPERTY()
	AActor* InitialGround;
	float InitialObstacleHeight;
	float NewObstacleHeight;

	UFUNCTION()
	void OnPlayerJumpStarted();

	UFUNCTION()
	void OnPlayerDidDoubleJump();

	void CheckForLanding();
	void AwardPoints(float AirTime);
	void DetectObstacleAtJumpStart();
	void CheckForNewObstacle();
};
