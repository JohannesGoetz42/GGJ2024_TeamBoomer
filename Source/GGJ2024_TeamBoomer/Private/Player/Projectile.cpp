// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectile.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerPawn.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root component");

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Projectile mesh");
	ProjectileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionObjectType(ECC_Pawn);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::HandleCollision);
	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(true);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Collision box");
	CollisionBox->AttachToComponent(ProjectileMesh, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::HandleCollision);
}

AProjectile* AProjectile::SpawnProjectile(UWorld* World, TSubclassOf<AProjectile> ProjectileClass, APawn* Instigator,
                                          AActor* Owner,
                                          const FVector& SourceLocation, const FVector& TargetLocation)
{
	if (!ensure(ProjectileClass))
	{
		return nullptr;
	}

	const AProjectile* DefaultProjectile = ProjectileClass.GetDefaultObject();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Instigator;
	SpawnParameters.Owner = Owner;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SourceLocation);

	FVector Impulse;
	if (TargetLocation == SourceLocation)
	{
		const FRotator ShootForwardRotator = FRotator(25.0f, 0.0f, 0.0f);
		Impulse = ShootForwardRotator.RotateVector(FVector(DefaultProjectile->ProjectileSpeed, 0.0f, 0.0f));
	}
	else
	{
		FVector Direction = TargetLocation - SourceLocation;
		Direction.Z = SourceLocation.Z;
		const float X = Direction.Size2D(); // do this BEFORE normalization!!!
		Direction.Normalize();

		FQuat Quat = FQuat::FindBetweenNormals(Direction, FVector::ForwardVector);
		FVector Axis;
		float Yaw;
		Quat.ToAxisAndAngle(Axis, Yaw);
		SpawnTransform.SetRotation(FRotator(0.0f, Yaw, 0.0f).Quaternion());
		Yaw = FMath::RadiansToDegrees(Yaw);
		Yaw *= -Axis.Z;

		const float G = -World->GetGravityZ();
		const float Y = TargetLocation.Z - SourceLocation.Z;

		const float V = DefaultProjectile->ProjectileSpeed;
		const float RootPart = FMath::Sqrt(FMath::Pow(V, 4) - G * (G * FMath::Square(X) + 2 * Y * FMath::Square(V)));
		const float Angle1 = FMath::RadiansToDegrees(FMath::Atan((FMath::Square(V) - RootPart) / (G * X)));
		const float Angle2 = FMath::RadiansToDegrees(FMath::Atan((FMath::Square(V) + RootPart) / (G * X)));

		float Angle = FMath::Abs(Angle1) < FMath::Abs(Angle2) ? Angle1 : Angle2;
		Angle = FMath::Clamp(Angle, -90.0f, 90.0f);

		Impulse = FRotator(Angle, Yaw, 0.0f).RotateVector(FVector::ForwardVector * DefaultProjectile->ProjectileSpeed);
	}

	AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform, SpawnParameters);
	// if spawned by pawn, disable the collision box
	if (Instigator)
	{
		SpawnedProjectile->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SpawnedProjectile->ProjectileMesh->AddImpulse(Impulse, NAME_None, true);
	SpawnedProjectile->PlaySound(EProjectileSoundType::PST_Shoot);
	return SpawnedProjectile;
}

void AProjectile::HandleCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	// if this projectile is not shot by a pawn, only destroy when a pawn is hit
	if (!GetInstigator() && !OtherActor->IsA<APawn>())
	{
		return;
	}

	PlaySound(EProjectileSoundType::PST_Hit);

	if (OverlappedComponent == CollisionBox)
	{
		if (APlayerPawn* Player = Cast<APlayerPawn>(OtherActor))
		{
			Player->ReceiveDamage(TearFluidCost);
		}
	}

	Destroy();
}

FVector AProjectile::GetImpulse() const
{
	return (ProjectileMesh->GetComponentLocation() - PreviousTickLocation) * Mass;
}

void AProjectile::PlaySound(EProjectileSoundType SoundType) const
{
	if (const TArray<TObjectPtr<USoundWave>>* SelectedSounds = SoundData.GetSoundsByType(SoundType))
	{
		const TArray<TObjectPtr<USoundWave>>& Sounds = *SelectedSounds;

		if (Sounds.IsEmpty())
		{
			return;
		}

		const int32 SoundIndex = FMath::RandRange(0, Sounds.Num() - 1);;
		if (USoundWave* Sound = Sounds[SoundIndex])
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, ProjectileMesh->GetComponentLocation(),
			                                      ProjectileMesh->GetComponentRotation());
		}
	}
}

void AProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PreviousTickLocation = ProjectileMesh->GetComponentLocation();
}
