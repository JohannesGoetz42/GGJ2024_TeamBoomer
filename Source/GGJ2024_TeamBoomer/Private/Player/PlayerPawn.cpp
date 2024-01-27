// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/Projectile.h"

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

	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CharacterMesh->SetCollisionObjectType(ECC_Pawn);
	CharacterMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
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

void APlayerPawn::Shoot()
{
	if (ensure(ProjectileClass))
	{
		AProjectile::SpawnProjectile(GetWorld(), ProjectileClass, CharacterMesh->GetComponentTransform(),
		                             BaseSpeed + MovementInput);
	}
}

void APlayerPawn::StartJump()
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Implement APlayerPawn::StartJump!"));
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(INPUT_MOVEMENT_LEFT_RIGHT, this, &APlayerPawn::MoveLeftRight);
	PlayerInputComponent->BindAxis(INPUT_MOVEMENT_FWD_BWD, this, &APlayerPawn::MoveForwardBackward);
	PlayerInputComponent->BindAction(INPUT_SHOOT, EInputEvent::IE_Pressed, this, &APlayerPawn::Shoot);
	PlayerInputComponent->BindAction(INPUT_JUMP, EInputEvent::IE_Released, this, &APlayerPawn::StartJump);
}
