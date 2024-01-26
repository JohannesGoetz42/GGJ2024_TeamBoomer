// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

#define INPUT_MOVEMENT_FWD_BWD "MovementForwardBackward"
#define INPUT_MOVEMENT_LEFT_RIGHT "MovementLeftRight"

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
	FVector BaseSpeed = FVector(100.0f, 0.0f, 0.0f);	
	UPROPERTY(EditDefaultsOnly)
	float MaxMovementSpeed = 50.0f; 
	
	FVector MovementInput;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveLeftRight(float AxisValue);
	void MoveForwardBackward(float AxisValue);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
