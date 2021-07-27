// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "XPlayerController.generated.h"

class UXItem;

UCLASS()
class EXPLORE_API AXPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<UXItem*> InventoryItems;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddInventoryItem(UXItem* Item, int32 ItemCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UXItem*> GetInventoryItems();
};
