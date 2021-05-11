// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "Abilities/XAbilitySystemComponent.h"

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
				UE_LOG(LogTemp, Warning, TEXT("CAN AFFORD!! ADDING TO RETURNED ARRAY %s"), *Ability->GetName());
				MatchingAbilities.Add(Cast<UXGameplayAbility>(Ability));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't afford!"));
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
					GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("%s"), *NewCDO->Next->GetName()));
					return NewCDO->Next;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid CDO...??"));
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Not found")));


	return nullptr;
}
