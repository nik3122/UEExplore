// Fill out your copyright notice in the Description page of Project Settings.


#include "XCharacterBase.h"

// Sets default values
AXCharacterBase::AXCharacterBase()
{
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UXAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UXAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

float AXCharacterBase::GetHealth()
{
	if (!AttributeSet)
		return 1.f;
	return AttributeSet->GetHealth();
}

float AXCharacterBase::GetStamina()
{
	if (!AttributeSet)
		return 1.f;
	return AttributeSet->GetStamina();
}

void AXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AttributeSet->InitHealth(100.0f);
	AttributeSet->InitStamina(50.0f);
}

void AXCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ability system"));
	}
}


void AXCharacterBase::AddStartupGameplayAbilities()
{
	for(TSubclassOf<UXGameplayAbility>& GameplayAbility: StartingGameplayAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GameplayAbility));
	}
	// TArray<FGameplayAbilitySpec>& MyAbilities = AbilitySystemComponent->GetActivatableAbilities();
	//
	// for(int32 Index = 0; Index != MyAbilities.Num(); ++Index)
	// {
	// 	
	// 	FGameplayAbilitySpec& strt = MyAbilities[Index];
	// 	
	// 	UE_LOG(LogTemp, Warning,TEXT("%s"), *strt.GetDebugString());
	// }
}

UXAbilitySystemComponent* AXCharacterBase::GetAbilitySystemComponent() const
{
 	return AbilitySystemComponent;
}

void AXCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TArray<FGameplayAbilitySpec> ActivatableAbilities = AbilitySystemComponent->GetActivatableAbilities();
	for(const FGameplayAbilitySpec Ability: ActivatableAbilities)
	{
		for(FGameplayTag Tag: Ability.Ability->AbilityTags)
		{
			TagContainer.AddTag(Tag);
		}
	}
}

void AXCharacterBase::GetAffordableAbilities(TArray<UXGameplayAbility*>& MatchingAbilities) const
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetAffordableAbilities(MatchingAbilities);
		
	}
}

void AXCharacterBase::GetAffordableAbilitiesByTag(FGameplayTagContainer TagContainer,
                                                  TArray<UXGameplayAbility*>& MatchingAbilities) const
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetAffordableAbilitiesByTag(TagContainer, MatchingAbilities);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I do not have an ASC"));
	}
}

TSubclassOf <UXGameplayAbility> AXCharacterBase::GetNextAbilityByClass(const TSubclassOf<UXGameplayAbility> AbilityClass) const
{
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("Found"));
		return AbilitySystemComponent->GetNextAbilityByClass(AbilityClass);
	}

	return nullptr;
}

