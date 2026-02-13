#include "BaseAICharacter.h"
#include "Survival/Character/Race.h"
#include "Survival/Character/Player/SurvivalCharacter.h"

ABaseAICharacter::ABaseAICharacter()
{
	SetType(ERace::Buffalo);
}

ETargetType ABaseAICharacter::GetFocusType_Implementation() const
{
	return ETargetType::Enemy;
}

ABaseAICharacter* ABaseAICharacter::GetFocusedEnemy_Implementation()
{
	return this;
}