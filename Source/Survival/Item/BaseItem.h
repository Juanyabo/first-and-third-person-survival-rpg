// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Survival/Item/Item.h"
#include "Survival/Interface/FocusTarget.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class SURVIVAL_API ABaseItem : public AActor, public IFocusTarget
{
	GENERATED_BODY()

	UPROPERTY()
	float BaseItemWeight;
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();
	
	UFUNCTION()
	void UpdateQuantity(const int Amount);

	UFUNCTION()
	void UpdateWeight();
	
	UFUNCTION(BlueprintGetter)
	FText GetItemName() const;
	
	UFUNCTION(BlueprintGetter)
	int GetQuantity() const;
	
	UFUNCTION(BlueprintGetter)
	float GetWeight() const;

	UFUNCTION(BlueprintGetter)
	bool GetIsStackable() const;

	UFUNCTION(BlueprintGetter)
	UMaterialInterface* GetRenderMaterial() const;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item", meta = (AllowPrivateAccess = "true"))
	FItem ItemData;

	virtual void BeginPlay() override;

	virtual ETargetType GetFocusType_Implementation() const override;

	virtual ABaseItem* GetFocusedItem_Implementation() override;
	
};
