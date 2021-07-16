#pragma once

#include "CoreMinimal.h"
#include "XItem.h"
#include "XWeapon.generated.h"

UCLASS()
class EXPLORE_API UXWeapon : public UXItem
{
	GENERATED_BODY()

public:
	UXWeapon()
	{
		ItemType = UXAssetManager::WeaponItemType;
	}		
};

