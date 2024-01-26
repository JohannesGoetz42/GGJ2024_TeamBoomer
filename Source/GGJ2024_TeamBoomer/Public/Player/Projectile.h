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

protected:
	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 500.0f;
	UPROPERTY(EditDefaultsOnly)
	float MaxLifetime = 5.0f;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	float CurrentLifeTime;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
