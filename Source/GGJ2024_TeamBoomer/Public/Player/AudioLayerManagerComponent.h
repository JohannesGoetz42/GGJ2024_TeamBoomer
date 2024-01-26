// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "AudioLayerManagerComponent.generated.h"


class UAudioLayerComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GGJ2024_TEAMBOOMER_API UAudioLayerManagerComponent : public UAudioComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	
	UPROPERTY()
	TObjectPtr<UAudioLayerComponent> MainTrack;
	UPROPERTY()
	TArray<TObjectPtr<UAudioLayerComponent>> CurrentLayers;
};
