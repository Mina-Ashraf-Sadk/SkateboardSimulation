#include "Core/SkateboarderCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

ASkateboarderCharacter::ASkateboarderCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialize movement component properties
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;

	// Create and attach the movement component
	SkateMovementComponent = CreateDefaultSubobject<USkateboardMovementComponent>(TEXT("SkateMovementComponent"));
}

void ASkateboarderCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the movement component with this character
	if (SkateMovementComponent)
	{
		SkateMovementComponent->Initialize(this);
	}

	// Setup enhanced input mapping context
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void ASkateboarderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateboarderCharacter::Move);
		EnhancedInput->BindAction(LeanAction, ETriggerEvent::Triggered, this, &ASkateboarderCharacter::Lean);
		EnhancedInput->BindAction(PushAction, ETriggerEvent::Triggered, this, &ASkateboarderCharacter::Push);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ASkateboarderCharacter::PerformJump);
	}
}

///////////////////////////
// Input Action Handlers //
///////////////////////////

void ASkateboarderCharacter::Move(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	if (SkateMovementComponent)
	{
		SkateMovementComponent->HandleMoveInput(MoveValue);
	}
}

void ASkateboarderCharacter::Lean(const FInputActionValue& Value)
{
	float LeanValue = Value.Get<float>();
	if (SkateMovementComponent)
	{
		SkateMovementComponent->HandleLeanInput(LeanValue);
	}
}

void ASkateboarderCharacter::Push(const FInputActionValue& Value)
{
	if (SkateMovementComponent)
	{
		SkateMovementComponent->HandlePush();
	}
}

void ASkateboarderCharacter::PerformJump()
{
	if (SkateMovementComponent)
	{
		SkateMovementComponent->HandleJump();
	}
}

void ASkateboarderCharacter::ExecutePush()
{
	if (SkateMovementComponent)
	{
		SkateMovementComponent->ExecutePush();
	}
}
