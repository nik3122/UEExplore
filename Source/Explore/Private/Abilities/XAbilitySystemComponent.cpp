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
		UGameplayAbility * Ability = FoundSpec->Ability;
		if (Ability->CheckCost(FoundSpec->Handle, AbilityActorInfo.Get()))
		{
			MatchingAbilities.Add(Cast<UXGameplayAbility>(Ability));
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
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Ability not found")));
	return nullptr;
}

UXAbilityFlow* UXAbilitySystemComponent::
GiveAbilityFlow(const FAbilityFlowSpec& FlowSpec)
{
	UXAbilityFlow* NewFlow = NewObject<UXAbilityFlow>(this, FlowSpec.AbilityFlow->GetClass());
	ActivatableAbilityFlows.Emplace(FlowSpec.Name, FlowSpec);
	return NewFlow;
}

UXAbilityFlow* UXAbilitySystemComponent::GetAbilityFlowInstance(const FName Name)
{
	FAbilityFlowSpec* Spec = ActivatableAbilityFlows.Find(Name);
	if (Spec)
	{
		return Spec->AbilityFlow;
	}
	return nullptr;
}

bool UXAbilitySystemComponent::ExecuteAbilityFlowByName(const FName Name)
{
	UXAbilityFlow* AbilityFlow = GetAbilityFlowInstance(Name);

	if (!AbilityFlow)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("[UXAbilitySystemComponent::TickExecuteAbilityFlow] Attempted to Execute a non granted AbilityFlow.")
		);
		return false;
	}
	
	const TSubclassOf<UXGameplayAbility> Ability = AbilityFlow->GetAbilityAtCurrentIndex();

	if(!Ability)
	{
		return false;
	}
	
	const bool AbilityActivated = TryActivateAbilityByClass(Ability);
	if(AbilityActivated)
	{
		// We tick the execution flow only if the ability was executed.
		// The flow is considered interrupted otherwise and must reset to 0.
		AbilityFlow->TickExecutionIndex();

	} else
	{
		AbilityFlow->ResetExecutionIndex();
	}
	return AbilityActivated;
}

void UXAbilitySystemComponent::ResetAllExecutionIndices()
{
	for (TPair<FName, FAbilityFlowSpec>& Kvp : ActivatableAbilityFlows)
	{
		const int32 ExecutionIndex = Kvp.Value.AbilityFlow->GetCurrentExecutionIndex();
		if (ExecutionIndex != 0)
		{
			Kvp.Value.AbilityFlow->ResetExecutionIndex();
		}
	}
}