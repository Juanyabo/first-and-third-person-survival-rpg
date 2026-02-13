#pragma once

#include "TargetType.generated.h"

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Enemy UMETA(DisplayName = "Enemy"),
	Item UMETA(DisplayName = "Item"),
	Weapon UMETA(DisplayName = "Weapon")
};