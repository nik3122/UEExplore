#pragma once
#include "Explore/Explore.h"

#include "XTypes.generated.h"


USTRUCT(BlueprintType)
struct EXPLORE_API FXItemSlot
{
	GENERATED_BODY()

	FXItemSlot(): SlotIndex(-1) {}

	FXItemSlot(const uint32 InSlotIndex, const FPrimaryAssetType& InItemType):
		SlotIndex(InSlotIndex),
		ItemType(InItemType) {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SlotIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetType ItemType;

	friend uint32 GetTypeHash(const FXItemSlot& Key)
	{
		uint32 Hash = 0;

		Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
		Hash = HashCombine(Hash, static_cast<uint32>(Key.SlotIndex));
		return Hash;
	}

	/** Equality operators */
	bool operator==(const FXItemSlot& Other) const
	{
		return ItemType == Other.ItemType && SlotIndex == Other.SlotIndex;
	}
	bool operator!=(const FXItemSlot& Other) const
	{
		return !(*this == Other);
	}
};

USTRUCT(BlueprintType)
struct EXPLORE_API FXItemData
{
	GENERATED_BODY()

	FXItemData(): ItemCount(1) {}
	FXItemData(const int32 InItemCount): ItemCount(InItemCount) {}
	
	/** The number of instances of this item in the inventory, can never be below 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemCount;
};
