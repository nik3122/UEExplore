// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XGameplayAbility.h"

#include "XAbilitySystemComponent.generated.h"

/**
* Subclass of ability system component with game-specific data
* Most games will need to make a game-specific subclass to provide utility functions
*/
UCLASS()
class EXPLORE_API UXAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void GetAffordableAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities);

	/* Get the class of the next ability linked to the one given. Useful to chain abilities as combos from AI. */
	TSubclassOf<UXGameplayAbility> GetNextAbilityByClass(TSubclassOf<UXGameplayAbility> AbilityClass);
};