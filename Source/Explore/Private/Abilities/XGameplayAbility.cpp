// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/XGameplayAbility.h"


UXGameplayAbility* UXGameplayAbility::GetNextAbility()
{
	return _Next;
}

FGameplayTagContainer UXGameplayAbility::GetAbilityTags() const
{
	ApplyGameplayEffectToTarget()
	return AbilityTags;
}

FGameplayTag UXGameplayAbility::GetRangeTags() const
{
	for(FGameplayTag Tag: AbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Range"))))
		{
			return Tag;		
		}
	}
	return FGameplayTag::EmptyTag;
}
