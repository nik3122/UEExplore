// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/XGameplayAbility.h"
#include "AbilitySystemComponent.h"


TArray<FActiveGameplayEffectHandle> UXGameplayAbility::ApplyEffectContainer(const FGameplayEventData& EventData)
{
	FXGameplayEffectContainer* EffectContainer = EffectContainerMap.Find(EventData.EventTag);
	TArray<FActiveGameplayEffectHandle> AllEffects;

	if (EffectContainer)
	{
		if (EffectContainer->TargetGameplayEffectClasses.IsValidIndex(0))
		{
			for (TSubclassOf<UGameplayEffect>& EffectClass : EffectContainer->TargetGameplayEffectClasses)
			{
				const FGameplayEffectSpecHandle Handle = MakeOutgoingGameplayEffectSpec(EffectClass);
				AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(Handle, EventData.TargetData));
			}
		}
	}

	return AllEffects;
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
