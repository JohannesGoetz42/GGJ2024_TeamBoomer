// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root component");

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Camera boom");
	SpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("Player camera");
	PlayerCamera->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>("Character mesh");
	CharacterMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(BaseSpeed * DeltaTime);
	CharacterMesh->AddRelativeLocation(MovementInput * DeltaTime);
}

void APlayerPawn::MoveLeftRight(float AxisValue)
{
	MovementInput.Y = AxisValue * MaxMovementSpeed;
}

void APlayerPawn::MoveForwardBackward(float AxisValue)
{
	MovementInput.X = AxisValue * MaxMovementSpeed;
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MovementLeftRight", this, &APlayerPawn::MoveLeftRight);
	PlayerInputComponent->BindAxis("MovementForwardBackward", this, &APlayerPawn::MoveForwardBackward);
}
