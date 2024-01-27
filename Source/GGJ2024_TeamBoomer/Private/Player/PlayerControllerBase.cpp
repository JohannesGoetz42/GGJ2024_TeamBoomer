// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerBase.h"

#include "Blueprint/UserWidget.h"

FVector APlayerControllerBase::GetCursorWorldLocation() const
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Camera, false, HitResult))
	{
		return HitResult.Location;
	}

	return FVector::Zero();
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(CursorWidgetClass))
	{
		SetShowMouseCursor(true);
		UUserWidget* CursorWidget = CreateWidget(this, CursorWidgetClass);
		SetMouseCursorWidget(EMouseCursor::Default, CursorWidget);
	}
}
