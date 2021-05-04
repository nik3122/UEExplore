// Fill out your copyright notice in the Description page of Project Settings.


#include "XCharacterBase.h"

// Sets default values
AXCharacterBase::AXCharacterBase()
{
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UXAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}


