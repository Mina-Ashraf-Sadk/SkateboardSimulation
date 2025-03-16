#include "Components/ScoreComponent.h"
#include "Core/SkateboarderCharacter.h"
#include "Components/SkateboardMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PlayerScore = 0;
	JumpStartTime = 0.f;
	StyleScore = 0;
	ObstacleBonus = 0.f;
	InitialGround = nullptr;
	InitialObstacleHeight = 0.f;
	NewObstacleHeight = 0.f;
}

void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ASkateboarderCharacter>(GetOwner());
	if (!PlayerCharacter)
	{
		return;
	}

	USkateboardMovementComponent* MovementComponent = PlayerCharacter->FindComponentByClass<USkateboardMovementComponent>();
	if (MovementComponent)
	{
		MovementComponent->OnJumpStart.AddDynamic(this, &UScoreComponent::OnPlayerJumpStarted);
		MovementComponent->OnDoubleJumped.AddDynamic(this, &UScoreComponent::OnPlayerDidDoubleJump);
	}
}

void UScoreComponent::OnPlayerJumpStarted()
{
	JumpStartTime = GetWorld()->GetTimeSeconds();
	StyleScore = 0;
	ObstacleBonus = 0.f;
	InitialGround = nullptr;
	InitialObstacleHeight = 0.f;
	NewObstacleHeight = 0.f;

	DetectObstacleAtJumpStart();

	GetWorld()->GetTimerManager().SetTimer(LandingCheckTimerHandle, this, &UScoreComponent::CheckForLanding, 0.1f, true);
}

void UScoreComponent::OnPlayerDidDoubleJump()
{
	StyleScore++;
}

void UScoreComponent::DetectObstacleAtJumpStart()
{
	if (!PlayerCharacter)
	{
		return;
	}

	FVector Start = PlayerCharacter->GetActorLocation();
	FVector End = Start - FVector(0, 0, 500.f);
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		InitialGround = Hit.GetActor();
		InitialObstacleHeight = Hit.ImpactPoint.Z;
		
		// Debug
		//DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 5.f, FColor::Blue, false, 2.f);
	}
}

void UScoreComponent::CheckForNewObstacle()
{
	if (!PlayerCharacter)
	{
		return;
	}

	FVector Start = PlayerCharacter->GetActorLocation();
	FVector End = Start - FVector(0, 0, 500.f);
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* CurrentObstacle = Hit.GetActor();

		if (CurrentObstacle && CurrentObstacle != InitialGround)
		{
			// Player cleared an obstacle
			NewObstacleHeight = Hit.ImpactPoint.Z;
			float Clearance = NewObstacleHeight - InitialObstacleHeight;

			if (Clearance > 0)
			{
				ObstacleBonus += Clearance * 0.2f; // Reward based on height difference
			}

			// Debug
			//DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Green, false, 0.1f, 0, 2.f);
		}
	}
}

void UScoreComponent::CheckForLanding()
{
	if (!PlayerCharacter || !PlayerCharacter->GetCharacterMovement())
	{
		return;
	}

	CheckForNewObstacle();

	if (!PlayerCharacter->GetCharacterMovement()->IsFalling())
	{
		float AirTime = GetWorld()->GetTimeSeconds() - JumpStartTime;
		AwardPoints(AirTime);
		GetWorld()->GetTimerManager().ClearTimer(LandingCheckTimerHandle);
	}
}

void UScoreComponent::AwardPoints(float AirTime)
{
	// No reward for insignificant jumps (low airtime, no spins, no obstacle clearance)
	if (AirTime < 1.5f  && ObstacleBonus == 0.f)
	{
		OnScoreUpdated.Broadcast(PlayerScore);
		return;
	}

	int32 BasePoints = FMath::RoundToInt(10 * ((AirTime * AirTime) + (AirTime * StyleScore)));
	int32 TotalPoints = BasePoints + FMath::RoundToInt(ObstacleBonus);
	PlayerScore += TotalPoints;
	OnScoreUpdated.Broadcast(PlayerScore);
}
