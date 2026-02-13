// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UBorder;
class UVerticalBox;
class UTextBlock;
class UHorizontalBox;
class UImage;
class UItemModal;
class UItemInfo;
class ABaseItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSelected, int, ItemIndex);

UCLASS()
class SURVIVAL_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UBorder* Inventory;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UVerticalBox* InventoryBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* TabName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* TabImages;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* ItemTitles;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UVerticalBox* ItemBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UItemModal* ItemModal;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item Slot")
	TSubclassOf<UItemInfo> ItemWidget;

public:

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemSelected OnInventoryItemSelected;

	UFUNCTION()
	void AddItem(const ABaseItem* Item, const int InventoryIndex) const;

	UFUNCTION()
	void RequestItemDetails(const int ItemIndex, const bool IsSelected);

	UFUNCTION()
	void ShowItemDetails(UMaterialInterface* Material);

protected:

	virtual void NativeConstruct() override;
	
};
