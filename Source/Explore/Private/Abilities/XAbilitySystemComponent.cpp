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

void UXAbilitySystemComponent::GetAffordableAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities)
{
	TArray<FGameplayAbilitySpec*> FoundSpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, FoundSpecs);

	for (FGameplayAbilitySpec* FoundSpec : FoundSpecs)
	{
		{
			UGameplayAbility * Ability = FoundSpec->Ability;
			if (Ability->CheckCost(FoundSpec->Handle, AbilityActorInfo.Get()))
			{
				MatchingAbilities.Add(Cast<UXGameplayAbility>(Ability));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Can't afford Ability: %s"), *Ability->GetName());
			}
		}
	}
}

TSubclassOf<UXGameplayAbility> UXAbilitySystemComponent::GetNextAbilityByClass(TSubclassOf<UXGameplayAbility> AbilityClass)
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
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid CDO...??"));
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Ability not found")));
	return nullptr;
}