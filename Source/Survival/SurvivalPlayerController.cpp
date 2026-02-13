// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ASurvivalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContexts, 0);
		}
	}
}