// Fill out your copyright notice in the Description page of Project Settings.


#include "Kula.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKula::AKula()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);

	
}

// Called when the game starts or when spawned
void AKula::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKula::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, KulaFX, GetActorLocation());
}

void AKula::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();
}

// Called every frame
//void AKula::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

