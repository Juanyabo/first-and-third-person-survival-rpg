// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfo.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Survival/Item/BaseItem.h"

void UItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Button->SetVisibility(ESlateVisibility::Visible);
	ItemBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	StackSize->SetVisibility(ESlateVisibility::Collapsed);
	IsEquipped->SetVisibility(ESlateVisibility::Hidden);
	Button->OnClicked.AddDynamic(this, &UItemInfo::SelectItem);
	Button->OnUnhovered.AddDynamic(this, &UItemInfo::OnUnhovered);

	Index = 0;
}

void UItemInfo::OnHovered()
{
	FLinearColor HoveredColor = Button->GetBackgroundColor();
	HoveredColor.A = 1.0f;
	Button->SetBackgroundColor(HoveredColor);
	Name->SetColorAndOpacity(EStyleColor::Black);
	StackSize->SetColorAndOpacity(EStyleColor::Black);
	Type->SetColorAndOpacity(EStyleColor::Black);
	Weight->SetColorAndOpacity(EStyleColor::Black);
	OnItemSelected.Broadcast(Index, true);
}

void UItemInfo::OnUnhovered()
{
	FLinearColor UnHoveredColor = Button->GetBackgroundColor();
	UnHoveredColor.A = 0.0f;
	Button->SetBackgroundColor(UnHoveredColor);
	Name->SetColorAndOpacity(EStyleColor::White);
	StackSize->SetColorAndOpacity(EStyleColor::White);
	Type->SetColorAndOpacity(EStyleColor::White);
	Weight->SetColorAndOpacity(EStyleColor::White);
	OnItemSelected.Broadcast(Index, false);
}

void UItemInfo::SelectItem()
{
	OnHovered();
}

void UItemInfo::UpdateItem(const ABaseItem* Item) const
{
	if (!Name->GetText().EqualTo(Item->GetItemName()))
	{
		Name->SetText(Item->GetItemName());
	}

	if (Item->GetQuantity() > 1)
	{
		StackSize->SetText(FText::AsNumber(Item->GetQuantity()));
		StackSize->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	Weight->SetText(FText::AsNumber(Item->GetWeight()));
	
}

void UItemInfo::SetIndex(const int NewIndex)
{
	Index = NewIndex;
}