#include "Core/SkateboardSimGameMode.h"
#include "Engine/World.h"
#include "TimerManager.h"

ASkateboardSimGameMode::ASkateboardSimGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ASkateboardSimGameMode::BeginPlay()
{
	Super::BeginPlay();
	RemainingTime = GameDuration;
	// Start the game countdown timer
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ASkateboardSimGameMode::DecreaseGameTime, 1.0f, true);
}

void ASkateboardSimGameMode::DecreaseGameTime()
{
	RemainingTime--;

	OnTimeUpdated.Broadcast(RemainingTime);

	if (RemainingTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
		OnGameOver.Broadcast();
	}
}
