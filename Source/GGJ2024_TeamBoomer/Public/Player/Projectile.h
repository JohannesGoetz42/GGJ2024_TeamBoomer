// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GGJ2024_TEAMBOOMER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AProjectile();

	static AProjectile* SpawnProjectile(UWorld* World, TSubclassOf<AProjectile> ProjectileClass, const FTransform& SpawnTransform, const FVector& SourceVelocity);

protected:
	UPROPERTY(EditDefaultsOnly)
	FVector ProjectileImpulse = FVector(1500.0f, 0.0f, 500.0f);
	UPROPERTY(EditDefaultsOnly)
	float MaxLifetime = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	float CurrentLifeTime;
};
