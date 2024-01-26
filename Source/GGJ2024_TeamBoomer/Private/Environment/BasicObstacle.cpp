// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/BasicObstacle.h"

// Sets default values
ABasicObstacle::ABasicObstacle()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root component");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// collision
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECC_Destructible);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABasicObstacle::HandleObstacleHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Obstacle hit: %s"), *GetName()));
	Destroy();
}

// Called when the game starts or when spawned
void ABasicObstacle::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABasicObstacle::HandleObstacleHit);
}
