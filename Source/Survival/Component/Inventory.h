// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class ABaseItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, ABaseItem*, Item, int, InventoryIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemData, UMaterialInterface*, Material);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVAL_API UInventory : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TSet<ABaseItem*> Items;
	
public:

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnItemData OnItemData;

	UInventory();

	void AddItem(ABaseItem* Item);

	UFUNCTION()
	void NotifyItemSelected(const int Index);
	
};
