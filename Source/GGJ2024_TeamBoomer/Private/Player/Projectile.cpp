// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root component");
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Projectile mesh");
	ProjectileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionObjectType(ECC_Pawn);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(true);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::HandleCollision);
}

AProjectile* AProjectile::SpawnProjectile(UWorld* World, TSubclassOf<AProjectile> ProjectileClass, APawn* Instigator,
                                          const FVector& SourceLocation, const FVector& TargetLocation,
                                          const FVector& SourceVelocity)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Instigator;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SourceLocation);	
	FVector Direction = TargetLocation - SourceLocation;
	Direction.Z = SourceLocation.Z;
	SpawnTransform.SetRotation(FQuat::FindBetweenVectors(FVector::ForwardVector, Direction));
	
	AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform, SpawnParameters);
	const FVector Impulse = SourceVelocity + SpawnedProjectile->GetActorRotation().RotateVector(
		SpawnedProjectile->ProjectileImpulse);
	SpawnedProjectile->ProjectileMesh->AddImpulse(Impulse, NAME_None, true);
	return SpawnedProjectile;
}

void AProjectile::HandleCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	Destroy();
}
