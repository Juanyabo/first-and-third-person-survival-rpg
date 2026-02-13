#pragma once

#include "CombatStyle.h"
#include "WeaponType.h"
#include "WeaponList.generated.h"

USTRUCT(BlueprintType)
struct FWeaponList
{
	GENERATED_BODY()

	FWeaponList();
	
	UPROPERTY(BlueprintReadOnly)
	TMap<EWeaponType, ECombatStyle> Weapon;
};
