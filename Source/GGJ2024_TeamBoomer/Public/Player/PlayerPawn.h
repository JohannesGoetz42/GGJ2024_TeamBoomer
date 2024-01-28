// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/AppliesPhysicsImpulse.h"
#include "PlayerPawn.generated.h"

#define INPUT_MOVEMENT_FWD_BWD "MovementForwardBackward"
#define INPUT_MOVEMENT_LEFT_RIGHT "MovementLeftRight"
#define INPUT_SHOOT "Shoot"
#define INPUT_JUMP "Jump"

#define SOCKETS_CENTER FName("Center")

class USphereComponent;
class UPlayerAnimBP;
class USplineComponent;
class AProjectile;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTearFluidAmountChangedDelegate);

UENUM()
enum class EPlayerPawnSoundType : uint8
{
	PPST_TakeDamage UMETA(DisplayName = "Take damage"),
	PPST_Jump UMETA(DisplayName = "Jump"),
	PPST_JumpEnd UMETA(DisplayName = "Jump end"),
	PPST_FillTear UMETA(DisplayName = "Fill tear"),
	PPST_Laugh UMETA(DisplayName = "Laugh")
};

USTRUCT()
struct FAnimationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> JumpAnimation;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> ShootAnimation;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> RollLoop;
};

USTRUCT()
struct FPlayerSoundData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> TakeDamageSounds;
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> JumpSounds;
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> JumpEndSounds;
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> TearFill;
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> Laugh;

	const TArray<TObjectPtr<USoundWave>>* GetSoundsByType(EPlayerPawnSoundType SoundType) const
	{
		switch (SoundType)
		{
		case EPlayerPawnSoundType::PPST_TakeDamage:
			return &TakeDamageSounds;
		case EPlayerPawnSoundType::PPST_Jump:
			return &JumpSounds;
		case EPlayerPawnSoundType::PPST_JumpEnd:
			return &JumpEndSounds;
		case EPlayerPawnSoundType::PPST_FillTear:
			return &TearFill;
		case EPlayerPawnSoundType::PPST_Laugh:
			return &Laugh;
		}
		return nullptr;
	}
};

UCLASS()
class GGJ2024_TEAMBOOMER_API APlayerPawn : public APawn, public IAppliesPhysicsImpulse
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	void HandleGameEnd();

protected:
	UPROPERTY(EditDefaultsOnly)
	FPlayerSoundData SoundData;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> CollisionSphere;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> PlayerCamera;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> RollingMovementSound;
	UPROPERTY(EditDefaultsOnly)
	FAnimationData AnimationData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaximumTearFluid = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CurrentTearFluid = 10;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameOverScreenClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HighScoreScreenClass;
	UPROPERTY(EditDefaultsOnly)
	float MaxMovementSpeed = 50.0f;
	UPROPERTY(EditDefaultsOnly)
	float JumpStrength = 100.0f;
	UPROPERTY(EditDefaultsOnly)
	/** How long it takes for one tear fluid unit to decay */
	float TearFluidDecay = 1.0f;
	UPROPERTY(BlueprintAssignable)
	FTearFluidAmountChangedDelegate OnTearFluidAmountChanged;
	UPROPERTY(EditDefaultsOnly)
	float Mass = 10.0f;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentScore;

	FTimerHandle RestoreMovementTimer;
	FVector MovementInput;
	FVector PreviousTickLocation;
	float TearDecayIntervalTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RestoreMovement();
	void AddTearFluid(int32 AddedAmount);
	void RemoveTearFluidAmount(int32 RemovedAmount);
	void ReceiveDamage(int32 DamageAmount);

protected:
	bool bIsGameOver;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void MoveLeftRight(float AxisValue);
	void MoveForwardBackward(float AxisValue);
	void Shoot();
	void StartJump()
	{
		PlayAnimation(AnimationData.JumpAnimation);
		PlaySound(EPlayerPawnSoundType::PPST_Jump);
		RollingMovementSound->SetVolumeMultiplier(0.0f);
	}
	void PlayAnimation(UAnimSequence* Animation, float PlaybackSpeed = 1.0f);
	void SetRestoreMovementTimer(float Delay);
	void HandleGameOver();

	virtual FVector GetImpulse() const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	void PlaySound(EPlayerPawnSoundType SoundType) const;
};
