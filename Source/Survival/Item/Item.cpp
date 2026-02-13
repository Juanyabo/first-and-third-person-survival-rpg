#include "Item.h"

FItem::FItem()
{
	IsStackable = true;
	Quantity = 1;
	Image = nullptr;
	Mesh = nullptr;
	Weight = 0.0f;
	RenderMaterial = nullptr;
}

