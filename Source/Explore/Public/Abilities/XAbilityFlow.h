// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "XGameplayAbility.h"
#include "UObject/NoExportTypes.h"
#include "XAbilityFlow.generated.h"

/**
 * A container for data necessary to define a series of Abilities
 * that can be chained in a flow (combo), and keep track of its execution state.
 */
UCLASS(Blueprintable)
class EXPLORE_API UXAbilityFlow : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
	UXAbilityFlow(): ExecutionIndex(0) {};

public:
	 virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer AbilityFlowTags;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentExecutionIndex() const;

	/** Return the list of abilities that make up the flow*/
	const TArray<TSubclassOf<UXGameplayAbility>>& GetActivatableAbilities() const
	{
		return Abilities;
	}

	TArray<TSubclassOf<UXGameplayAbility>>& GetActivatableAbilities()
	{
		return Abilities;
	}

	// Return the ability at current index of execution
	TSubclassOf<UXGameplayAbility> GetAbilityAtCurrentIndex();

	/** Reset ExecutionIndex back to 0 */
	UFUNCTION()
	void ResetExecutionIndex();

	UFUNCTION(BlueprintCallable)
	int32 TickExecutionIndex();
	
protected:
	/** The Abilities that make up the flow. EXPECTED TO BE IN EXECUTION ORDER */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TSubclassOf<UXGameplayAbility>> Abilities;

private:

	/** When the AbilityFLow is instanced, holds an index to the currently executing Ability */
	UPROPERTY()
	int32 ExecutionIndex;
};
