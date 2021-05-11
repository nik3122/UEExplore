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
		UE_LOG(LogTemp, Warning, TEXT("Now Loaded %s"), *_Next->GetName());
	}
}
