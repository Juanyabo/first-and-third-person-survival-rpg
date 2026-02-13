// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Survival/Interface/TargetType.h"
#include "FocusTarget.generated.h"

class ABaseAICharacter;
class ABaseItem;
class AWeapon;

UINTERFACE(MinimalAPI)
class UFocusTarget : public UInterface
{
	GENERATED_BODY()
};

class SURVIVAL_API IFocusTarget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ETargetType GetFocusType() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ABaseAICharacter* GetFocusedEnemy();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ABaseItem* GetFocusedItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AWeapon* GetFocusedWeapon();
};
