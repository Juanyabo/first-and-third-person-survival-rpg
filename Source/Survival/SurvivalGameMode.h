#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SurvivalGameMode.generated.h"

class USurvivalHUD;

UCLASS()
class SURVIVAL_API ASurvivalGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menus", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USurvivalHUD> HUDWidget;

	UPROPERTY()
	USurvivalHUD* HUDInstance;

public:

	UFUNCTION()
	USurvivalHUD* GetHUDInstance() const;

protected:

	virtual void BeginPlay() override;
	
};
