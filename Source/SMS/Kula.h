// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kula.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UParticleSystem;


UCLASS()
class SMS_API AKula : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKula();

	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* KulaFX;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		USoundBase* OverlapSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

public:	
	// Called every frame
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
