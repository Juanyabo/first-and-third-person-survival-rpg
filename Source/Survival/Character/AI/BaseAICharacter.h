// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/Character/BaseCharacter.h"
#include "Survival/Interface/FocusTarget.h"
#include "BaseAICharacter.generated.h"

UCLASS()
class SURVIVAL_API ABaseAICharacter : public ABaseCharacter, public IFocusTarget
{
	GENERATED_BODY()

public:

	ABaseAICharacter();

protected:

	virtual ETargetType GetFocusType_Implementation() const override;

	virtual ABaseAICharacter* GetFocusedEnemy_Implementation() override;
	
};
