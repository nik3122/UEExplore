// Fill out your copyright notice in the Description page of Project Settings.


#include "XPlayerController.h"
#include "Characters/XCharacterBase.h"
#include "Items/XItem.h"

bool AXPlayerController::AddInventoryItem(UXItem* Item, int32 ItemCount)
{
	if(!Item)
		return false;

	InventoryItems.Add(Item);
	return true;
}

TArray<UXItem*> AXPlayerController::GetInventoryItems()
{
	return InventoryItems;
}

