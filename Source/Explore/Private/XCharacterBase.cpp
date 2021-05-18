// Fill out your copyright notice in the Description page of Project Settings.


#include "XCharacterBase.h"

#include "Explore/Explore.h"

// Sets default values
AXCharacterBase::AXCharacterBase()
{
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UXAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UXAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AXCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
		InitializeAttributes();

		if (InputComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("We have an InputCompnent: %"), *InputComponent->GetName());
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("No ASC at PossessedBy() time"));
	}
}

/*
void AXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AbilitySystemComponent)
	{
		const FGameplayAbilityInputBinds AbilityBinds("Confirm", "Cancel", "EGdAbilityInputID", static_cast<int32>(EGdAbilityInputID::Confirm), static_cast<int32>(EGdAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, AbilityBinds);
	}
}
*/

UXAbilitySystemComponent* AXCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/** Currently returning only GAS Tags. This is not technically correct. Should be refactored. */
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

int32 AXCharacterBase::GetCurrentLevel()
{
	return 1;
}

float AXCharacterBase::GetHealth() const
{
	if (!AttributeSet)
		return 1.f;
	return AttributeSet->GetHealth();
}

float AXCharacterBase::GetStamina() const
{
	if (!AttributeSet)
		return 1.f;
	return AttributeSet->GetStamina();
}

void AXCharacterBase::AddStartupGameplayAbilities()
{
	for(TSubclassOf<UXGameplayAbility>& GameplayAbility: StartingAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(GameplayAbility, GetCurrentLevel(), INDEX_NONE, this));
	}
}

void AXCharacterBase::InitializeAttributes()
{
	if(DefaultAttributesEffect && AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributesEffect, GetCurrentLevel(), EffectContextHandle);

		if (EffectSpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
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
		UE_LOG(LogTemp, Warning, TEXT("No ASC at GetAffordableAbilitiesByTag() time"));
	}
}

TSubclassOf <UXGameplayAbility> AXCharacterBase::GetNextAbilityByClass(const TSubclassOf<UXGameplayAbility> AbilityClass) const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->GetNextAbilityByClass(AbilityClass);
	}

	return nullptr;
}

