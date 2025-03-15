#include "SkateBoarder/Public/Components/ScoreComponent.h"


UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PlayerScore = 0;
}

void UScoreComponent::AddPoints(int32 Points)
{
	PlayerScore += Points;
	OnScoreUpdated.Broadcast(PlayerScore);
}