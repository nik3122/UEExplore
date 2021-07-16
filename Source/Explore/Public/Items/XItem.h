// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "XAssetManager.h"
#include "Abilities/XGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "XItem.generated.h"

class UXAbilitySystemComponent;
UCLASS()
class EXPLORE_API UXItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UXItem()
	{
		ItemType = UXAssetManager::BaseItemType;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "UI"))
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "UI"))
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "UI"))
	UTexture2D* Thumbnail; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UXGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> ActorClass;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;	
};
