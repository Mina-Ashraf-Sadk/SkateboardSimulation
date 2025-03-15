#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "ScoreComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdated, int32, NewScore);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SKATEBOARDER_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScoreComponent();

	void AddPoints(int32 Points);

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreUpdated OnScoreUpdated;

private:
	int32 PlayerScore;
};
