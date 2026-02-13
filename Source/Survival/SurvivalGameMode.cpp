// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Survival/UI/SurvivalHUD.h"

void ASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidget))
	{
		HUDInstance = CreateWidget<USurvivalHUD>(GetWorld(), HUDWidget);
		HUDInstance->AddToViewport();
	}
}

USurvivalHUD* ASurvivalGameMode::GetHUDInstance() const
{
	return HUDInstance;
}