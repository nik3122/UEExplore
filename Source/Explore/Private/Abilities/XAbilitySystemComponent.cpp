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
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UXGameplayAbility>(ActiveAbility));
		}
	}
}

TSubclassOf<UXGameplayAbility> UXAbilitySystemComponent::GetNextAbilityByClass(const TSubclassOf<UXGameplayAbility> AbilityClass)
{
	const UXGameplayAbility* const AbilityClassCDO = AbilityClass.GetDefaultObject();
	
	for (const FGameplayAbilitySpec& FoundSpec: ActivatableAbilities.Items)
	{
		if (FoundSpec.Ability == AbilityClassCDO)
		{
			TSubclassOf<UXGameplayAbility> NewClass = FoundSpec.Ability->GetClass();
			if (IsValid(NewClass))
			{
				const UXGameplayAbility* NewCDO = NewClass.GetDefaultObject();
				if (IsValid(NewCDO))
				{
					return NewCDO->Next;
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Ability not found")));
	return nullptr;
}
