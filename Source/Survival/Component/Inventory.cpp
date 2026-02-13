#include "Inventory.h"
#include "Survival/Item/BaseItem.h"

UInventory::UInventory()
{
	
}

void UInventory::AddItem(ABaseItem* Item)
{
	int InventoryIndex = 0;
	
	if (Item->GetIsStackable())
	{
		for (ABaseItem* ExistingItem : Items)
		{
			if (ExistingItem->GetItemName().EqualTo(Item->GetItemName()))
			{
				ExistingItem->UpdateQuantity(Item->GetQuantity());
				ExistingItem->UpdateWeight();
				OnItemAdded.Broadcast(ExistingItem, InventoryIndex);
				Item->Destroy();
				return;
			}
			
			++InventoryIndex;
		}
	}
	
	Items.Add(Item);
	OnItemAdded.Broadcast(Item, InventoryIndex);
	Item->Destroy();
}

void UInventory::NotifyItemSelected(const int Index)
{
	if (IsValid(Items.Array()[Index]->GetRenderMaterial()))
	{
		OnItemData.Broadcast(Items.Array()[Index]->GetRenderMaterial());
	}
}