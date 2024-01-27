// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicObstacle.generated.h"

UCLASS()
class GGJ2024_TEAMBOOMER_API ABasicObstacle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasicObstacle();

protected:	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UFUNCTION()
	void HandleObstacleHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
