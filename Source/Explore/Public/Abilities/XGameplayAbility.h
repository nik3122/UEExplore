// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "XGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORE_API UXGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadonly, EditAnywhere)
	TSubclassOf<UXGameplayAbility> Next;

	UFUNCTION(BlueprintCallable)
	UXGameplayAbility* GetNextAbility();

	UFUNCTION(BlueprintCallable)
	void LoadNextAbility();

	/** Expose AbilityTags to Blueprint */
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetAbilityTags() const;

private:
	UPROPERTY()
	UXGameplayAbility* _Next;
};
