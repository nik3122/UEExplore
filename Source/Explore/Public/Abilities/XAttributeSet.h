// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "XAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORE_API UXAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
};
