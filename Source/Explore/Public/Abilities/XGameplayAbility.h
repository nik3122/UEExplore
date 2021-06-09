// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "XAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "Explore/Explore.h"

#include "XGameplayAbility.generated.h"

/**
 * Expand base GameplayAbility with utilities for input binding, applying
 * GameplayEffects and querying Ability tags.
 */
UCLASS()
class EXPLORE_API UXGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	/** DEPRECATED: used for old combo system and currently only used by AI. Will be removed. */
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Combo")
	TSubclassOf<UXGameplayAbility> Next;

	/** DEPRECATED: used for old combo system and currently only used by AI. Will be removed. */
	UFUNCTION(BlueprintCallable)
	UXGameplayAbility* GetNextAbility();

	/** Bind Ability activation to this Input */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	EAbilityInput InputID;

	/** Gameplay Effects to apply for a given tag */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effects")
	TMap<FGameplayTag, FXGameplayEffectContainer> EffectContainerMap;

	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual TArray<FActiveGameplayEffectHandle>  ApplyEffectContainer (FGameplayTag EffectContainerTag, const FGameplayEventData& EventData);

	/** Expose AbilityTags to Blueprint */
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetAbilityTags() const;

	/** Get tag that describes the range type of this ability */
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetRangeTags() const;

private:
	/** DEPRECATED: used for old combo system and currently only used by AI. Will be removed. */
	UPROPERTY()
	UXGameplayAbility* _Next;
};
