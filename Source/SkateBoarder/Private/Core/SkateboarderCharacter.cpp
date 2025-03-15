


#include "SkateBoarder/Public/Core/SkateboarderCharacter.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>


ASkateboarderCharacter::ASkateboarderCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create a Camera Component
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(RootComponent);

    // Create a Score Component
   // ScoreComp = CreateDefaultSubobject<UScoreComponent>(TEXT("ScoreComp"));

    // Configure Movement
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->BrakingFrictionFactor = 1.5f; // Helps slow down naturally
    GetCharacterMovement()->GroundFriction = 2.0f; // Keeps player from slipping
}

void ASkateboarderCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ASkateboarderCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ApplyFriction(DeltaTime);
}

void ASkateboarderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind Movement Inputs
    PlayerInputComponent->BindAxis("MoveForward", this, &ASkateboarderCharacter::MoveForward);
    PlayerInputComponent->BindAxis("Turn", this, &ASkateboarderCharacter::Turn);

    // Bind Actions
    PlayerInputComponent->BindAction("Push", IE_Pressed, this, &ASkateboarderCharacter::Push);
    PlayerInputComponent->BindAction("SlowDown", IE_Pressed, this, &ASkateboarderCharacter::SlowDown);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASkateboarderCharacter::PerformJump);
}

void ASkateboarderCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void ASkateboarderCharacter::Turn(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerYawInput(Value * TurnSpeed * GetWorld()->GetDeltaSeconds());
    }
}

void ASkateboarderCharacter::Push()
{
    float NewSpeed = GetCharacterMovement()->MaxWalkSpeed + PushForce;
    GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(NewSpeed, 600.0f, MaxSpeed);
}

void ASkateboarderCharacter::SlowDown()
{
    float NewSpeed = GetCharacterMovement()->MaxWalkSpeed - SlowDownRate;
    GetCharacterMovement()->MaxWalkSpeed = FMath::Max(NewSpeed, 600.0f);
}

void ASkateboarderCharacter::PerformJump()
{
    if (CanJump())
    {
        FVector JumpImpulse = FVector(600.f, 0.f, 350.f); // Slight forward boost
        LaunchCharacter(JumpImpulse, false, true);
    }
}

void ASkateboarderCharacter::ApplyFriction(float DeltaTime)
{
    float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
    float NewSpeed = FMath::FInterpTo(CurrentSpeed, 600.0f, DeltaTime, 1.5f);
    GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}