// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

class UBoxComponent;

UENUM()
enum ETrapType
{
	TT_None UMETA(DisplayName = "None / Blueprint implemented"),
	TT_Projectile_Forward UMETA(DispayName = "Projectile forward"),
	TT_Projectile_Aimed UMETA(DispayName = "Projectile aimed"),
	TT_Projectile_HandGrab UMETA(DispayName = "Hand grab")
};

UCLASS()
class GGJ2024_TEAMBOOMER_API ATrap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrap();

protected:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETrapType> TrapType;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> SpawnedMesh;
	UPROPERTY(EditAnywhere)
	/** The aim distance (Projectile forward) / MovementDistance (Hand grab) */
	float Distance = 100.0f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> TriggerSound;

	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteTrap();
	UFUNCTION()
	void TriggerTrap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                 const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
