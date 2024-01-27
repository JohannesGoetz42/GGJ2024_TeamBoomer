// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

#define INPUT_MOVEMENT_FWD_BWD "MovementForwardBackward"
#define INPUT_MOVEMENT_LEFT_RIGHT "MovementLeftRight"
#define INPUT_SHOOT "Shoot"
#define INPUT_JUMP "Jump"

class UAudioLayerManagerComponent;
class USplineComponent;
class AProjectile;
class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTearFluidAmountChangedDelegate);

UCLASS()
class GGJ2024_TEAMBOOMER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> CharacterMesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> PlayerCamera;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioLayerManagerComponent> AudioLayerManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaximumTearFluid = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CurrentTearFluid = 10;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
	float MaxMovementSpeed = 50.0f;
	UPROPERTY(EditDefaultsOnly)
	float JumpStrength = 100.0f;
	UPROPERTY(BlueprintAssignable)
	FTearFluidAmountChangedDelegate OnTearFluidAmountChanged;

	FVector MovementInput;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddTearFluid(int32 AddedAmount);

protected:
	void MoveLeftRight(float AxisValue);
	void MoveForwardBackward(float AxisValue);
	void Shoot();
	void StartJump();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
};
