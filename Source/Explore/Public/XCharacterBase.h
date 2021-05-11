// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "Abilities/XAttributeSet.h"
#include "GameFramework/Character.h"
#include "Abilities/XAbilitySystemComponent.h"
#include "Abilities/XGameplayAbility.h"
#include "CombatSystem/XCombo.h"

#include "XCharacterBase.generated.h"


UCLASS()
class EXPLORE_API AXCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXCharacterBase();

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	float GetStamina();

	FGameplayAbilitySpec* MyAbspec;
	


protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;

	/** The component used to handle ability system interactions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UXAbilitySystemComponent* AbilitySystemComponent;
	
	/** List of attributes modified by the ability system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UXAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UXCombo>> Combos;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UXGameplayAbility>> StartingGameplayAbilities;

	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();
	
	// Implement IAbilitySystemInterface
	virtual class UXAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	void GetAffordableAbilities(FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities) const;

	/* Exposes method to get the class of the ability linked to the one give, if exists. */

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UXGameplayAbility> GetNextAbilityByClass(const TSubclassOf<UXGameplayAbility> AbilityClass) const;
};
