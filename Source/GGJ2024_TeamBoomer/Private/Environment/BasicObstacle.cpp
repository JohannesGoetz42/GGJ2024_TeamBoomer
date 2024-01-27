// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/BasicObstacle.h"

#include "Player/PlayerPawn.h"

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
	APlayerPawn* Player = Cast<APlayerPawn>(OtherActor);
	if (!Player)
	{
		Player = Cast<APlayerPawn>(OtherActor->GetInstigator());
	}

	if(ensure(Player))
	{
		Player->AddTearFluid(TearFluidReward);
	}
	
	Destroy();
}

// Called when the game starts or when spawned
void ABasicObstacle::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABasicObstacle::HandleObstacleHit);
}
