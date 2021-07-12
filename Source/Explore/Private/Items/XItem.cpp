#include "Items/XItem.h"

FPrimaryAssetId UXItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
