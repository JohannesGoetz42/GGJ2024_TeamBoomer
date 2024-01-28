// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AppliesPhysicsImpulse.h"
#include "Projectile.generated.h"

class UBoxComponent;

UENUM()
enum class EProjectileSoundType : uint8
{
	PST_Shoot UMETA(DisplayName = "Shoot"),
	PST_Hit UMETA(DisplayName = "Hit")
};

USTRUCT()
struct FProjectileSounds
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> ShootSounds;
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundWave>> HitSounds;

	const TArray<TObjectPtr<USoundWave>>* GetSoundsByType(EProjectileSoundType SoundType) const
	{
		switch (SoundType)
		{
		case EProjectileSoundType::PST_Shoot:
			return &ShootSounds;
		case EProjectileSoundType::PST_Hit:
			return &HitSounds;
		}
		return nullptr;
	}
};

UCLASS(Abstract)
class GGJ2024_TEAMBOOMER_API AProjectile : public AActor, public IAppliesPhysicsImpulse
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	static AProjectile* SpawnProjectile(UWorld* World, TSubclassOf<AProjectile> ProjectileClass, APawn* Instigator,
	                                    AActor* Owner, const FVector& SourceLocation, const FVector& TargetLocation);
	int32 GetTearFluidCost() const { return TearFluidCost; }

protected:
	UPROPERTY(EditDefaultsOnly)
	FProjectileSounds SoundData;
	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 1500.0f;
	UPROPERTY(EditDefaultsOnly)
	float MaxLifetime = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	float Mass = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	int32 TearFluidCost = 10;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CollisionBox;

	float CurrentLifeTime;
	FVector PreviousTickLocation;

	UFUNCTION()
	void HandleCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual FVector GetImpulse() const override;
	void PlaySound(EProjectileSoundType SoundType) const;
	virtual void Tick(float DeltaSeconds) override;
};
