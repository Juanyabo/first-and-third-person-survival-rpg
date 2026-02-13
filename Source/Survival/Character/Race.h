#pragma once

#include "Race.generated.h"

UENUM(BlueprintType)
enum class ERace : uint8
{
	Human UMETA(DisplayName = "Human"),
	Buffalo UMETA(DisplayName = "Buffalo"),
};