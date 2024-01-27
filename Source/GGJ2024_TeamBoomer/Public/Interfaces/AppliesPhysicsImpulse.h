// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AppliesPhysicsImpulse.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAppliesPhysicsImpulse : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GGJ2024_TEAMBOOMER_API IAppliesPhysicsImpulse
{
	GENERATED_BODY()

public:
	virtual FVector GetImpulse() const = 0;
};
