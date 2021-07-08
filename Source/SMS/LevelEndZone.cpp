// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEndZone.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SMSCharacter.h"
#include "SMSGameMode.h"

// Sets default values
ALevelEndZone::ALevelEndZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;
	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelEndZone::HandleOverlap);

	Table = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table"));
	Table->SetVisibility(true);
	Table->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Table->SetCollisionResponseToAllChannels(ECR_Block);
	Table->SetupAttachment(RootComponent);

	Sofa = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sofa"));
	Sofa->SetVisibility(true);
	Sofa->SetupAttachment(Table);
	Sofa->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Sofa->SetCollisionResponseToAllChannels(ECR_Block);

	TV = CreateDefaultSubobject<UChildActorComponent>(TEXT("TV"));
	TV->SetVisibility(true);
	TV->SetupAttachment(Table);

}

// Called when the game starts or when spawned
void ALevelEndZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelEndZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASMSCharacter* MyPawn = Cast<ASMSCharacter>(OtherActor);

	if(MyPawn == nullptr) return;
	if (MyPawn->bHaveGoogles)
	{
		ASMSGameMode* GM = Cast<ASMSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleateMission(MyPawn);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}

	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"));
}

// Called every frame
void ALevelEndZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

