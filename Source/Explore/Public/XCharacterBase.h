// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/XAttributeSet.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "XCharacterBase.generated.h"


UCLASS()
class EXPLORE_API AXCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXCharacterBase();

protected:
	/** The component used to handle ability system interactions */
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
	
	/** List of attributes modified by the ability system */
	UPROPERTY() const UXAttributeSet* AttributeSet;
};
