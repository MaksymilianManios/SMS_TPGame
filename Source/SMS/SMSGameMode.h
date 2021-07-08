// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SMSGameMode.generated.h"


UCLASS(minimalapi)
class ASMSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "SpectCams")
	TSubclassOf<AActor> SpectatingViewpointClass;
	const EEndPlayReason::Type EndPlayReason = EEndPlayReason::Type::LevelTransition;

public:
	ASMSGameMode();

	void CompleateMission(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionComplited(APawn* InstigatorPawn);

};



