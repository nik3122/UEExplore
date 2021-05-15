// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/XGameplayAbility.h"


UXGameplayAbility* UXGameplayAbility::GetNextAbility()
{
	return _Next;
}

void UXGameplayAbility::LoadNextAbility()
{
	if (IsValid(Next))
	{
		_Next = Next.GetDefaultObject();
	}
}

FGameplayTagContainer UXGameplayAbility::GetAbilityTags() const
{
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
