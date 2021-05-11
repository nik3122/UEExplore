// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/XGameplayAbility.h"
#include "UObject/NoExportTypes.h"
#include "XCombo.generated.h"

/**
 * A UXCombo is a container of GAs that can be activated in sequence.
 */
UCLASS(Blueprintable)
class EXPLORE_API UXCombo : public UObject
{
	GENERATED_BODY()

public:
	UXCombo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="AttackCombo")
	TArray<TSubclassOf<UXGameplayAbility>> ComboAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 ComboId;
};
