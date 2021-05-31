// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "XAssetManager.h"
#include "Abilities/XGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "XItem.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORE_API UXItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UXItem()
	{
		ItemType = UXAssetManager::BaseItemType;
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UXGameplayAbility> GrantedAbility;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
