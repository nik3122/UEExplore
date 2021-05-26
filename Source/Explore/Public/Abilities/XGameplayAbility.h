// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Explore/Explore.h"

#include "XGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORE_API UXGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Combo")
	TSubclassOf<UXGameplayAbility> Next;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Input")
	EAbilityInput InputID;

	UFUNCTION(BlueprintCallable)
	UXGameplayAbility* GetNextAbility();

	/** Expose AbilityTags to Blueprint */
	UFUNCTION(BlueprintCallable)
	FGameplayTagContainer GetAbilityTags() const;

	/** Get tag that describes the range type of this ability */
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetRangeTags() const;


private:
	UPROPERTY()
	UXGameplayAbility* _Next;
};
