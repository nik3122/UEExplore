// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "Abilities/XAbilitySystemComponent.h"

void UXAbilitySystemComponent::GetAffordableAbilities(TArray<UXGameplayAbility*>& MatchingAbilities)
{
	for (const FGameplayAbilitySpec& Spec: ActivatableAbilities.Items)
	{
		UGameplayAbility* Ability = Spec.Ability;
		if (Ability->CheckCost(Spec.Handle, AbilityActorInfo.Get()))
		{
			MatchingAbilities.Add(Cast<UXGameplayAbility>(Ability));
		}
	}
}

void UXAbilitySystemComponent::GetAffordableAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities) const
{
	TArray<FGameplayAbilitySpec*> FoundSpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, FoundSpecs);

	for (FGameplayAbilitySpec* FoundSpec : FoundSpecs)
	{
		UGameplayAbility * Ability = FoundSpec->Ability;
		if (Ability->CheckCost(FoundSpec->Handle, AbilityActorInfo.Get()))
		{
			MatchingAbilities.Add(Cast<UXGameplayAbility>(Ability));
		}
	}
}

void UXAbilitySystemComponent::GetActiveAbilitiesByTag(const FGameplayTagContainer TagContainer,
	TArray<UXGameplayAbility*>& ActiveAbilities) const
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, AbilitiesToActivate, true);
	
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UXGameplayAbility>(ActiveAbility));
		}
	}
}
