// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	TObjectPtr<USphereComponent> CollisionSphere;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> PlayerCamera;
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

protected:	
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void MoveLeftRight(float AxisValue);
	void MoveForwardBackward(float AxisValue);
	void Shoot();
	void StartJump() { PlayAnimation(AnimationData.JumpAnimation); }
	void PlayAnimation(UAnimSequence* Animation, float PlaybackSpeed = 1.0f);
	void SetRestoreMovementTimer(float Delay);
	void HandleGameOver() const;

	virtual FVector GetImpulse() const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
};
