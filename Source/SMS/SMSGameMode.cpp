// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SMSGameMode.h"
#include "SMSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

ASMSGameMode::ASMSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASMSGameMode::CompleateMission(APawn* InstigatorPawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("1"));
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
		//UE_LOG(LogTemp, Warning, TEXT("2"));
		if (SpectatingViewpointClass)
		{
			//UE_LOG(LogTemp, Warning, TEXT("3"));
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			// Change view if any valid actor found
			if (ReturnedActors.Num() > 0)
			{
				AActor* NewViewTarget = ReturnedActors[0];

				APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 0.7f, EViewTargetBlendFunction::VTBlend_Cubic);
					
					PC->EndPlay(EndPlayReason);

				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass is a nullptr. Please update GameMode class with valid subclass. Cannot change spectating view target "))
		}
	}

	OnMissionComplited(InstigatorPawn);
}
