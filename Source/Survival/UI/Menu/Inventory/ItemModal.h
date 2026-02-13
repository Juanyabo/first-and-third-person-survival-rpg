// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemModal.generated.h"

class UImage;

UCLASS()
class SURVIVAL_API UItemModal : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* Image;

	UPROPERTY()
	UMaterialInterface* CurrentMaterial;
	
public:

	UFUNCTION()
	void SetPreviewItemData(UMaterialInterface* Material);
	
protected:

	virtual void NativeConstruct() override;
	
};
