// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Survival/Item/BaseItem.h"
#include "Survival/UI/Menu/Inventory/ItemInfo.h"
#include "ItemModal.h"

void  UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Inventory->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	InventoryBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TabName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	TabImages->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ItemTitles->SetVisibility(ESlateVisibility::HitTestInvisible);
	ItemBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
}

void UInventoryMenu::AddItem(const ABaseItem* Item, const int InventoryIndex) const
{
	if (IsValid(ItemWidget))
	{
		UItemInfo* ItemSlot = nullptr;
		
		if (Item->GetIsStackable())
		{
			ItemSlot = Cast<UItemInfo>(ItemBox->GetChildAt(InventoryIndex));
		}

		if (IsValid(ItemSlot))
		{
			ItemSlot->UpdateItem(Item);
		}
		else
		{
			ItemSlot = CreateWidget<UItemInfo>(GetWorld(), ItemWidget);
			ItemSlot->UpdateItem(Item);
			ItemBox->AddChild(ItemSlot);
			ItemSlot->SetIndex(ItemBox->GetChildIndex(ItemSlot));
			ItemSlot->OnItemSelected.AddDynamic(this, &UInventoryMenu::RequestItemDetails);
		}
	}
}

void UInventoryMenu::RequestItemDetails(const int ItemIndex, const bool IsSelected)
{
	if (IsSelected)
	{
		OnInventoryItemSelected.Broadcast(ItemIndex);
	}
	else
	{
		ItemModal->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryMenu::ShowItemDetails(UMaterialInterface* Material)
{
	if (IsValid(Material))
	{
		ItemModal->SetPreviewItemData(Material);
	}
}