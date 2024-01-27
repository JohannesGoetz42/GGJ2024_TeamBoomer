// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AudioLayerManagerComponent.h"

#include "Player/AudioLayerComponent.h"

void UAudioLayerManagerComponent::AddAudioLayer(USoundWave* SoundAsset)
{
	UAudioLayerComponent* AudioLayer = NewObject<UAudioLayerComponent>(GetOwner());
	AudioLayer->Sound = SoundAsset;
	AudioLayer->RegisterComponent();
	CurrentLayers.Add(SoundAsset, AudioLayer);
}

void UAudioLayerManagerComponent::RemoveAudioLayer(USoundWave* SoundAsset)
{
	TObjectPtr<UAudioLayerComponent> ActiveLayer;
	CurrentLayers.RemoveAndCopyValue(SoundAsset, ActiveLayer);
	if (ActiveLayer)
	{
		ActiveLayer->DestroyComponent();
	}
}