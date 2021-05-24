// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "Abilities/XAttributeSet.h"
#include "GameFramework/Character.h"
#include "Abilities/XAbilitySystemComponent.h"
#include "Abilities/XGameplayAbility.h"
#include "GameplayEffect.h"

#include "XCharacterBase.generated.h"

USTRUCT()
struct FAbilityFlowHandle
{
	GENERATED_BODY()

	FAbilityFlowHandle(): Handle(INDEX_NONE), AbilityFlow(nullptr)
	{
	};
	FAbilityFlowHandle(int32 InHandle, UXAbilityFlow* InFlow)
		: Handle(InHandle), AbilityFlow(InFlow)
	{

	}
	UPROPERTY()
	int32 Handle;

	UPROPERTY()
	UXAbilityFlow * AbilityFlow;
};


UCLASS()
class EXPLORE_API AXCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXCharacterBase();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;

	// Implement IAbilitySystemInterface
	virtual UXAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Implement IGameplayTagAssetInterface
	UFUNCTION(BlueprintCallable)
	virtual void GetOwnedGameplayTags (FGameplayTagContainer & TagContainer) const override;
	
	/** The component used to handle ability system interactions */
	UPROPERTY()
	UXAbilitySystemComponent* AbilitySystemComponent;
	
	/** List of attributes modified by the ability system 
	* WARNING: should not be UPROPERTY but without the engine crashes when initializing attributes.
	* Probably because the Enemy BP has some old cached data. Will have to fix it at some point.
	*/
	UPROPERTY()
	UXAttributeSet* AttributeSet;
    
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities)
    TSubclassOf<UGameplayEffect> DefaultAttributesEffect;
    
	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UXGameplayAbility>> StartingAbilities;
	
	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();

	/** Apply the DefaultGameplayEffect set on this Character */
	virtual void InitializeAttributes();

	/** Get current Character level. Mainly used by GAS */
	int32 GetCurrentLevel();
	
	/** Get abilities that this character can afford to activate */
	UFUNCTION(BlueprintCallable)
	void GetAffordableAbilities(TArray<UXGameplayAbility*>& MatchingAbilities) const;
	
	/** Get abilities that this character can afford to activate, by tag */
	UFUNCTION(BlueprintCallable)
	void GetAffordableAbilitiesByTag(FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities) const;

	/* Expose method to get the class of the ability linked to the one given, if exists. */
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UXGameplayAbility> GetNextAbilityByClass(const TSubclassOf<UXGameplayAbility> AbilityClass) const;

	// --------------------------------------
	//	ABILITY FLOW SYSTEM
	//
	//	This should probably be moved into the AbilitySystemComponent
	// --------------------------------------

	/** Map of available flows */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Abilities)
	TMap<FName, TSubclassOf<UXAbilityFlow>> AbilityFlowMap;

	UPROPERTY()
	/** A map of handles to the currently instantiated flows */
	TMap<FName, FAbilityFlowHandle> ActiveAbilityFlows;
	
	/* Create and return a new AbilityFlow**/
	UFUNCTION(BlueprintCallable)
	UXAbilityFlow* CreateAbilityFlowInstance(FName Name);

	/**
	* Main function of the flow system. Checks if a flow exists in Character's flow map. If not,
	* instantiates one and proceeds to activate the ability at the current ExecutionIndex, ticking
	* it or resetting it according to the success or not of the ability activation.
	*/
	UFUNCTION(BlueprintCallable)
	UXAbilityFlow* GetAbilityFlowInstance(FName Name);
	
	UFUNCTION(BlueprintCallable)
	bool TickExecuteAbilityFlow(FName Name);

	/** Reset ExecutionIndex on all active Flows*/
	UFUNCTION(BlueprintCallable)
	void ResetAllExecutionIndices();
};
