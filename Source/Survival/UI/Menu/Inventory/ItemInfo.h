// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfo.generated.h"

class UButton;
class UHorizontalBox;
class UImage;
class UTextBlock;
class ABaseItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSelected, int, Index, bool, IsSelected);

UCLASS()
class SURVIVAL_API UItemInfo : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* ItemBox;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Name;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* StackSize;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* IsEquipped;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Type;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Weight;

	UPROPERTY()
	int Index;

	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();
	
	UFUNCTION()
	void SelectItem();

public:

	UPROPERTY(BlueprintAssignable)
	FOnItemSelected OnItemSelected;

	UFUNCTION()
	void UpdateItem(const ABaseItem* Item) const;

	UFUNCTION(BlueprintSetter)
	void SetIndex(const int NewIndex);

protected:

	virtual void NativeConstruct() override;
	
};
