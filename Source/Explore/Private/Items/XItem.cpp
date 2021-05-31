// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/XItem.h"

FPrimaryAssetId UXItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
