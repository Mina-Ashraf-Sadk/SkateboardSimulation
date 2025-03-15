// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/AnimNotify_Push.h"

#include "Core/SkateboarderCharacter.h"

void UAnimNotify_Push::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		AActor* Owner = MeshComp->GetOwner();
		if (ASkateboarderCharacter* SkateChar = Cast<ASkateboarderCharacter>(Owner))
		{
			// Call the push execution function on the character.
			SkateChar->ExecutePush();
		}
	}
}
