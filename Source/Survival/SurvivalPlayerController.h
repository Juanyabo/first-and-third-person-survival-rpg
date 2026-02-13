// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SurvivalPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class SURVIVAL_API ASurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Input Mapping Context */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	UInputMappingContext* DefaultMappingContexts;
	
	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
	
};