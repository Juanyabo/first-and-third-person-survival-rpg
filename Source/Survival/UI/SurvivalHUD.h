#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SurvivalHUD.generated.h"

class UWidgetSwitcher;
class UOverlay;
class UVerticalBox;
class UTextBlock;
class UHorizontalBox;
class UProgressBar;
class UInventoryMenu;
class UInventory;
class ABaseItem;

UCLASS()
class SURVIVAL_API USurvivalHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOverlay* HUDOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UVerticalBox* EnemyBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* EnemyName;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UProgressBar* EnemyHealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* HealthBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UProgressBar* PlayerHealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* StaminaBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UProgressBar* PlayerStaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UVerticalBox* ObjectBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ObjectName;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Key;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Action;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UInventoryMenu* InventoryMenu;

	UPROPERTY()
	FText PreviousObjectName;

	UPROPERTY()
	UInventory* InventoryDisplayed;

	UFUNCTION()
	static void UpdateProgressBar(UProgressBar* ProgressBar, const int CurrentValue, const int MaxValue);
	
public:

	UFUNCTION()
	void HandleInventory(UInventory* Inventory);
	
	UFUNCTION()
	void UpdateEnemyHealthBar(const int CurrentHealth, const int MaxHealth, const FText Name);
	
	UFUNCTION()
	void UpdatePlayerHealthBar(const int CurrentHealth, const int MaxHealth, const FText Name);
	
	UFUNCTION()
	void UpdatePlayerSprintBar(const int CurrentStamina, const int MaxStamina);

	UFUNCTION()
	void UpdateItem(ABaseItem* Item, const bool IsItemInRange);
	
	UFUNCTION()
	void UpdateInventory(ABaseItem* Item, const int InventoryIndex);
	
	UFUNCTION()
	UInventoryMenu* GetInventoryMenu() const;
	
protected:

	virtual void NativeConstruct() override;
	
};
