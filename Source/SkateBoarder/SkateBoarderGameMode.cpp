// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateBoarderGameMode.h"
#include "SkateBoarderCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASkateBoarderGameMode::ASkateBoarderGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
