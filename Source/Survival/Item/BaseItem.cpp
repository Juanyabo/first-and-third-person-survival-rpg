#include "BaseItem.h"
#include "Survival/Character/Player/SurvivalCharacter.h"

ABaseItem::ABaseItem()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);
	StaticMesh->SetSimulatePhysics(true);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	ItemData.Mesh = StaticMesh->GetStaticMesh();
	BaseItemWeight = ItemData.Weight;
}

ETargetType ABaseItem::GetFocusType_Implementation() const
{
	return ETargetType::Item;
}

ABaseItem* ABaseItem::GetFocusedItem_Implementation()
{
	return this;
}

void ABaseItem::UpdateQuantity(const int Amount)
{
	ItemData.Quantity += Amount;
}

void ABaseItem::UpdateWeight()
{
	ItemData.Weight = BaseItemWeight * ItemData.Quantity;
}

FText ABaseItem::GetItemName() const
{
	return ItemData.Name;
}

int ABaseItem::GetQuantity() const
{
	return ItemData.Quantity;
}

float ABaseItem::GetWeight() const
{
	return ItemData.Weight;
}

bool ABaseItem::GetIsStackable() const
{
	return ItemData.IsStackable;
}

UMaterialInterface* ABaseItem::GetRenderMaterial() const
{
	return ItemData.RenderMaterial;
}