// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	None			UMETA(DisplayName="None"),
	Confirm			UMETA(DisplayName="Confirm"),
	Cancel			UMETA(DisplayName="Cancel"),
	LightAttack		UMETA(DisplayName="LightAttack"),
	StrongAttack	UMETA(DisplayName="StrongAttack"),
	Roll            UMETA(DisplayName="Roll"),
	Dash			UMETA(DisplayName="Dash")
};