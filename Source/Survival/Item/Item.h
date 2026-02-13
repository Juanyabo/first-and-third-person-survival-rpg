#pragma once

#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	FItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Image;

	UPROPERTY()
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> RenderMaterial;
	
};
