// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XGameplayAbility.h"

#include "XAbilitySystemComponent.generated.h"


/** Subclass of ability system component with game-specific data **/
UCLASS()
class EXPLORE_API UXAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/* Get all Abilities that can be activated given their cost */
	void GetAffordableAbilities(TArray<UXGameplayAbility*>& MatchingAbilities);

	/* Get Abilities that can be activated given their cost, filtering them by given tag. */
	void GetAffordableAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities) const;

	UFUNCTION(BlueprintCallable)
	void GetActiveAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& ActiveAbilities) const;
};