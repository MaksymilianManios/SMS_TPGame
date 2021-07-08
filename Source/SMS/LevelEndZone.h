// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelEndZone.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class SMS_API ALevelEndZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelEndZone();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Sofa;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Table;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UChildActorComponent* TV;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase* ObjectiveMissingSound;
};
