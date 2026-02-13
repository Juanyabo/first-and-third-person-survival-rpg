// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemModal.h"
#include "Components/Image.h"

void UItemModal::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(false);
	SetVisibility(ESlateVisibility::Hidden);

	CurrentMaterial = nullptr;
}

void UItemModal::SetPreviewItemData(UMaterialInterface* Material)
{
	if (CurrentMaterial != Material)
	{
		CurrentMaterial = Material;
		Image->SetBrushFromMaterial(Material);
	}
	SetVisibility(ESlateVisibility::HitTestInvisible);
}