#pragma once

#include "CharacterType.generated.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Humanoid UMETA(DisplayName = "Humanoid"),
	Beast UMETA(DisplayName = "Beast")
};