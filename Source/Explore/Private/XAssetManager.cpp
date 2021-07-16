// Fill out your copyright notice in the Description page of Project Settings.


#include "XAssetManager.h"

const FPrimaryAssetType UXAssetManager::BaseItemType = TEXT("BaseItem");
const FPrimaryAssetType UXAssetManager::WeaponItemType = TEXT("Weapon");

UXAssetManager& UXAssetManager::Get()
{
	UXAssetManager* This = Cast<UXAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		return *NewObject<UXAssetManager>(); // never calls this
	}}
