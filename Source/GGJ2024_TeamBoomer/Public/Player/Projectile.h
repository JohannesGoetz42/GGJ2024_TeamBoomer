// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AppliesPhysicsImpulse.h"
#include "Projectile.generated.h"

UCLASS()
class GGJ2024_TEAMBOOMER_API AProjectile : public AActor, public IAppliesPhysicsImpulse
{
private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	static AProjectile* SpawnProjectile(UWorld* World, TSubclassOf<AProjectile> ProjectileClass, APawn* Instigator,
	                                    const FVector& SourceLocation, const FVector& TargetLocation,
	                                    const FVector& SourceVelocity);
	int32 GetTearFluidCost() const { return TearFluidCost; }

protected:
	UPROPERTY(EditDefaultsOnly)
	FVector ProjectileImpulse = FVector(1500.0f, 0.0f, 500.0f);
	UPROPERTY(EditDefaultsOnly)
	float MaxLifetime = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	float Mass = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	int32 TearFluidCost = 10;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	float CurrentLifeTime;
	FVector PreviousTickLocation;

	UFUNCTION()
	void HandleCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual FVector GetImpulse() const override;
	virtual void Tick(float DeltaSeconds) override;
};
