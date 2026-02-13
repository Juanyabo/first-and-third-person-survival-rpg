// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalHUD.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Survival/Item/BaseItem.h"
#include "Menu/Inventory/InventoryMenu.h"
#include "Survival/Component/Inventory.h"

void USurvivalHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	WidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	HUDOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	EnemyBox->SetVisibility(ESlateVisibility::Collapsed);
	HealthBox->SetVisibility(ESlateVisibility::Collapsed);
	StaminaBox->SetVisibility(ESlateVisibility::Collapsed);
	ObjectBox->SetVisibility(ESlateVisibility::Collapsed);

	InventoryDisplayed = nullptr;
}

void USurvivalHUD::UpdateProgressBar(UProgressBar* ProgressBar, const int CurrentValue, const int MaxValue)
{
	const float HealthPercent = static_cast<float>(CurrentValue) / static_cast<float>(MaxValue);
	ProgressBar->SetPercent(HealthPercent);
}

void USurvivalHUD::HandleInventory(UInventory* Inventory)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		if (WidgetSwitcher->GetActiveWidget() != InventoryMenu)
		{
			InventoryDisplayed = Inventory;
			PlayerController->SetPause(true);
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetInputMode(FInputModeUIOnly());
			WidgetSwitcher->SetActiveWidgetIndex(1);
		}
		else
		{
			InventoryMenu->OnInventoryItemSelected.Clear();
			InventoryDisplayed->OnItemData.Clear();
			PlayerController->SetPause(false);
			PlayerController->SetShowMouseCursor(false);
			PlayerController->SetInputMode(FInputModeGameOnly());
			WidgetSwitcher->SetActiveWidgetIndex(0);
		}
	}
}

void USurvivalHUD::UpdateEnemyHealthBar(const int CurrentHealth, const int MaxHealth, const FText Name)
{
	UpdateProgressBar(EnemyHealthBar, CurrentHealth, MaxHealth);
	EnemyName->SetText(Name);
	EnemyBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (CurrentHealth <= 0)
	{
		EnemyBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USurvivalHUD::UpdatePlayerHealthBar(const int CurrentHealth, const int MaxHealth, const FText Name)
{
	UpdateProgressBar(PlayerHealthBar, CurrentHealth, MaxHealth);
	HealthBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (CurrentHealth <= 0)
	{
		HealthBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USurvivalHUD::UpdatePlayerSprintBar(const int CurrentStamina, const int MaxStamina)
{
	UpdateProgressBar(PlayerStaminaBar, CurrentStamina, MaxStamina);
	StaminaBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (CurrentStamina >= MaxStamina)
	{
		StaminaBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USurvivalHUD::UpdateItem(ABaseItem* Item, const bool IsItemInRange)
{
	if (IsValid(Item))
	{
		if (IsItemInRange)
		{
			ObjectBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			PreviousObjectName = ObjectName->GetText();
			if (!Item->GetItemName().EqualTo(PreviousObjectName))
			{
				ObjectName->SetText(Item->GetItemName());
			}
		}
		else
		{
			ObjectBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		ObjectBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void USurvivalHUD::UpdateInventory(ABaseItem* Item, const int InventoryIndex)
{
	InventoryMenu->AddItem(Item, InventoryIndex);
}

UInventoryMenu* USurvivalHUD::GetInventoryMenu() const
{
	return InventoryMenu;
}