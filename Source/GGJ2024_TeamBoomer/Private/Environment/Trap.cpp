// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Trap.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ATrap::ATrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root component");
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger box");
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TriggerSound = CreateDefaultSubobject<UAudioComponent>("Audio comp");
	TriggerSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TriggerSound->SetPaused(true);
}

void ATrap::TriggerTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult& SweepResult)
{
	FVector TargetLocation;
	switch (TrapType)
	{
	case TT_None:
		ExecuteTrap();
		break;
	case TT_Projectile_Forward:
		TargetLocation = Mesh->GetComponentLocation() + Mesh->GetForwardVector() * Distance;
		break;
	case TT_Projectile_Aimed:
		TargetLocation = OverlappedComponent->GetComponentLocation();
		break;
	case TT_Projectile_HandGrab:
		ensure(false); // implement this!!!
		break;
	}
	
	TriggerSound->SetPaused(false);
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::TriggerTrap);
}
