#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SkateboardSimGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdated, int32, RemainingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

UCLASS()
class SKATEBOARDER_API ASkateboardSimGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASkateboardSimGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Game Session")
	int32 GameDuration = 120; // Total time in seconds.

	int32 RemainingTime;
	FTimerHandle GameTimerHandle;

	void DecreaseGameTime();

public:
	UPROPERTY(BlueprintAssignable, Category = "Game Session")
	FOnTimeUpdated OnTimeUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Game Session")
	FOnGameOver OnGameOver;
};
