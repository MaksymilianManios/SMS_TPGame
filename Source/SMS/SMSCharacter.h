// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "SMSCharacter.generated.h"

UCLASS(config=Game)
class ASMSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ASMSCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jump")
	bool bJumpEffectAdded;	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "High Jump")
	bool bIsHighJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "No Jump")
	bool bIsNoJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jump Effect Timer")
	float JumpEffectTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gogle")
	bool bHaveGoogles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gogle")
	bool bIsDead;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION(BlueprintCallable)
	void ShowGogle();

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComp,class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult &SweepResult);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	void Jump();	

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

public:
	UFUNCTION(BlueprintCallable)
	void SetHighJump();

	UFUNCTION(BlueprintCallable)
	void SetNoJump();

	UFUNCTION(BlueprintCallable)
	void SetJumpToDefault();

	// void PickUpObjective();

	void RestratLevel();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* KulaDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* SpikesDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
    USoundBase* WaterDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* JumpSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* MoveSound;


	/*UPROPERTY(EditAnywhere, Category = "PLAYER HUD")
		TSubclassOf<UUserWidget> JumpBustUI_Class;
	UUserWidget* JumpBustUI;

	UPROPERTY(EditAnywhere, Category = "PLAYER HUD")
		TSubclassOf<UUserWidget> JumpNerfUI_Class;
	UUserWidget* JumpNerfUI;*/
};

