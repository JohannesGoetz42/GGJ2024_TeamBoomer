// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerBase.h"

#include "Blueprint/UserWidget.h"

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
