#include "RaceList.h"

FRaceList::FRaceList()
{
	Race.Add(ERace::Human, ECharacterType::Humanoid);
	Race.Add(ERace::Buffalo, ECharacterType::Beast);
}