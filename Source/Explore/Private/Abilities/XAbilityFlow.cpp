// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/XAbilityFlow.h"

void UXAbilityFlow::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
}

int32 UXAbilityFlow::GetCurrentExecutionIndex() const
{
	return ExecutionIndex;
}

TSubclassOf<UXGameplayAbility> UXAbilityFlow::GetAbilityAtCurrentIndex()
{
	if (Abilities.IsValidIndex(ExecutionIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Getting Ability at index: %d"), ExecutionIndex);
		TSubclassOf<UXGameplayAbility> Ability = Abilities[ExecutionIndex];
		UE_LOG(LogTemp, Warning, TEXT("ABILITYNAME: %s"), *Ability->GetName());
		return Ability;
	}
	UE_LOG(LogTemp, Warning, TEXT("BAD INDEX!!"));

	return Abilities[0];
}

void UXAbilityFlow::ResetExecutionIndex()
{
	UE_LOG(LogTemp, Warning, TEXT("Resetting FLOW to 0"));
	ExecutionIndex = 0;
}

int32 UXAbilityFlow::TickExecutionIndex()
{
	if (ExecutionIndex >= Abilities.Num() - 1)
	{
		ResetExecutionIndex();
	} else
	{
		ExecutionIndex += 1;
	}
	UE_LOG(LogTemp, Warning, TEXT("Ticking FLOW to %d"), GetCurrentExecutionIndex());
	return ExecutionIndex;
}

