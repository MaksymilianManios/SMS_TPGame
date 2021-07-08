// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gral.generated.h"

UCLASS()
class SMS_API AGral : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGral();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GogleComp")
	class UStaticMeshComponent* MeshComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
