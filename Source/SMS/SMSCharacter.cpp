// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SMSCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// ASMSCharacter

ASMSCharacter::ASMSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&ASMSCharacter::OnBeginOverlap);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	bIsDead = false;
	bHaveGoogles = false;
	bIsHighJump = false;
	bIsNoJump = false;
	bJumpEffectAdded = false;
	JumpEffectTimer = 0.0f;

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASMSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASMSCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASMSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASMSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASMSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASMSCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASMSCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASMSCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASMSCharacter::OnResetVR);
}

void ASMSCharacter::BeginPlay()
{
	Super::BeginPlay();
	

}

void ASMSCharacter::ShowGogle()
{
	bHaveGoogles = true;
}



void ASMSCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASMSCharacter::Jump()
{
	if(!bIsDead){
		Super::Jump();
		UGameplayStatics::PlaySound2D(this, JumpSound );
	}
}

void ASMSCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if(!bIsDead)Jump();
}

void ASMSCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}



void ASMSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASMSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASMSCharacter::MoveForward(float Value)
{
	
	if ((Controller != NULL) && (Value != 0.0f) && !bIsDead)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		// UGameplayStatics::PlaySound2D(this, MoveSound);
	}
}

void ASMSCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !bIsDead)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASMSCharacter::SetHighJump() {	
	GetCharacterMovement()->JumpZVelocity = 850.0f;	
}

void ASMSCharacter::SetNoJump() {	
	GetCharacterMovement()->JumpZVelocity = 100.0f;	
}

void ASMSCharacter::SetJumpToDefault() {
	bIsHighJump = false;
	bIsNoJump = false;
	bJumpEffectAdded = false;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
}

void ASMSCharacter::RestratLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()),false);
	
}

void ASMSCharacter::Tick(float DeltaTime)
{
	if(bJumpEffectAdded)
	{
		if (bIsHighJump) {
			SetHighJump();
			
		}else if (bIsNoJump) {
			SetNoJump();
			
		}
		JumpEffectTimer = 1000.0f;
		
		bJumpEffectAdded = false;
		
	}
	if(JumpEffectTimer>1)
	{
		JumpEffectTimer--;
	}
	else if (JumpEffectTimer == 1) {
		JumpEffectTimer--;
		
		SetJumpToDefault();
	}
	if (bIsDead)
	{
		//this->DisableInput(nullptr);
		GetMesh()->SetSimulatePhysics(true);
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ASMSCharacter::RestratLevel, 3.0f, false);
	}
	Super::Tick(DeltaTime);

}



void ASMSCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, 
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
								   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("JB")) {
		bJumpEffectAdded = true;
		bIsNoJump = false;
		bIsHighJump = true;		
		OtherActor->Destroy();
	}
	if (OtherActor->ActorHasTag("JD")) {
		bJumpEffectAdded = true;
		bIsHighJump = false;
		bIsNoJump = true;		
		OtherActor->Destroy();
	}
	if (OtherActor->ActorHasTag("KULA")) {
		bIsDead = true;
		UGameplayStatics::PlaySound2D(this, KulaDeath);
	}
	if (OtherActor->ActorHasTag("SPIKE")) {
		bIsDead = true;
		UGameplayStatics::PlaySound2D(this, SpikesDeath);
	}
	if (OtherActor->ActorHasTag("Water")) {
		bIsDead = true;
		UGameplayStatics::PlaySound2D(this, WaterDeath);
	}
}