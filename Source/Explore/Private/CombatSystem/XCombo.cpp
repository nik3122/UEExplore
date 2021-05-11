// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/XCombo.h"

UXCombo::UXCombo()
{
	for (TSubclassOf<UXGameplayAbility> ability : ComboAbilities)
	{
		UXGameplayAbility* instance = ability.GetDefaultObject();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *instance->GetName());
	}
}
	