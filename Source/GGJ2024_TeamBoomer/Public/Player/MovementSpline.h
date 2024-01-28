// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementSpline.generated.h"

class APlayerPawn;
class USplineComponent;

UCLASS()
class GGJ2024_TEAMBOOMER_API AMovementSpline : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovementSpline();

	void ControlPawn(APlayerPawn* PawnToControl);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> SplineComponent;
	UPROPERTY()
	TObjectPtr<APlayerPawn> ControlledPawn;

	float CurrentDuration;
	
	virtual void Tick(float DeltaSeconds) override;
};
