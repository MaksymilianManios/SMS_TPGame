// Fill out your copyright notice in the Description page of Project Settings.


#include "Gral.h"
#include "Components/StaticMeshComponent.h"
#include <SMS/SMSCharacter.h>

// Sets default values
AGral::AGral()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GogleSM"));
	MeshComp->SetVisibility(true);
}

// Called when the game starts or when spawned
void AGral::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGral::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGral::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

