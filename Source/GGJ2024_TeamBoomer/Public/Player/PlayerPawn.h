// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

#define INPUT_MOVEMENT_FWD_BWD "MovementForwardBackward"
#define INPUT_MOVEMENT_LEFT_RIGHT "MovementLeftRight"
#define INPUT_SHOOT "Shoot"
#define INPUT_JUMP "Jump"

class AProjectile;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GGJ2024_TEAMBOOMER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CharacterMesh;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly)
	FVector BaseSpeed = FVector(100.0f, 0.0f, 0.0f);	
	UPROPERTY(EditDefaultsOnly)
	float MaxMovementSpeed = 50.0f; 
	UPROPERTY(EditDefaultsOnly)
	float JumpStrength = 100.0f;
	
	FVector MovementInput;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void MoveLeftRight(float AxisValue);
	void MoveForwardBackward(float AxisValue);
	void Shoot();
	void StartJump();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
