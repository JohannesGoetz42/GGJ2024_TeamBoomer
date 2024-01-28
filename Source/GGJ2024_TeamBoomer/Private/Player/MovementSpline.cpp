// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MovementSpline.h"

#include "Components/SplineComponent.h"
#include "Player/PlayerPawn.h"

// Sets default values
AMovementSpline::AMovementSpline()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Movement Spline");
	SplineComponent->EditorUnselectedSplineSegmentColor = FColor::Green;
}

void AMovementSpline::ControlPawn(APlayerPawn* PawnToControl)
{
	if (ensure(PawnToControl))
	{
		ControlledPawn = PawnToControl;
		SetActorTickEnabled(true);
		CurrentDuration = 0.0f;
	}
}

// Called every frame
void AMovementSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ensure(ControlledPawn))
	{
		CurrentDuration += DeltaTime;
		if (CurrentDuration > SplineComponent->Duration)
		{
			ControlledPawn->HandleGameEnd();
			SetActorTickEnabled(false);
			return;
		}

		const FTransform SplineTransform = SplineComponent->GetTransformAtTime(
			CurrentDuration, ESplineCoordinateSpace::World,
			true);
		ControlledPawn->SetActorTransform(SplineTransform);
	}
}
