// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XAbilityFlow.h"
#include "XGameplayAbility.h"

#include "XAbilitySystemComponent.generated.h"


/** Subclass of ability system component with game-specific data **/
UCLASS()
class EXPLORE_API UXAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/* Get all Abilities that can be activated given their cost */
	void GetAffordableAbilities(TArray<UXGameplayAbility*>& MatchingAbilities);

	/* Get Abilities that can be activated given their cost, filtering them by given tag. */
	void GetAffordableAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities);

	/* Get the class of the next ability linked to the one given. Useful to chain abilities as combos. */
	TSubclassOf<UXGameplayAbility> GetNextAbilityByClass(TSubclassOf<UXGameplayAbility> AbilityClass);

	// --------------------------------------
	//	ABILITY FLOW SYSTEM
	// --------------------------------------
	
	/* Grants a new AbilityFlow **/
	UXAbilityFlow* GiveAbilityFlow(const FAbilityFlowSpec& FlowSpec);

	UFUNCTION(BlueprintCallable)
	UXAbilityFlow* GetAbilityFlowInstance(FName Name);

	/**
	* Main function of the flow system. Checks if a flow exists in Character's flow map. If not,
	* instantiates one and proceeds to activate the ability at the current ExecutionIndex, ticking
	* it or resetting it according to the success or not of the ability activation.
	*/
	UFUNCTION(BlueprintCallable)
	bool ExecuteAbilityFlowByName(FName Name);

	/** Reset ExecutionIndex on all active Flows*/
	void ResetAllExecutionIndices();

protected:
	
	UPROPERTY()
	/** A map of specs of the currently instantiated flows */
	TMap<FName, FAbilityFlowSpec> ActivatableAbilityFlows;
};