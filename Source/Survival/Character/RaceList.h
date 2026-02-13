#pragma once

#include "Race.h"
#include "CharacterType.h"
#include "RaceList.generated.h"

USTRUCT(BlueprintType)
struct FRaceList
{
	GENERATED_BODY()

	FRaceList();
	
	UPROPERTY(BlueprintReadOnly)
	TMap<ERace, ECharacterType> Race;
};
