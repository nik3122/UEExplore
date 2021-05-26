// Fill out your copyright notice in the Description page of Project Settings.


#include "XCharacterBase.h"

// Sets default values
AXCharacterBase::AXCharacterBase()
{
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UXAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent = CreateDefaultSubobject<UXAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
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

void AXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,
			FGameplayAbilityInputBinds(
FString("Confirm"),
FString("Cancel"),
		FString("EAbilityInput"),
static_cast<int32>(EAbilityInput::Confirm),
static_cast<int32>(EAbilityInput::Cancel)
		));
	}

}

void AXCharacterBase::AddStartupGameplayAbilities()
{
	for(TSubclassOf<UXGameplayAbility>& GameplayAbility: StartingAbilities)
	{
		if (GameplayAbility)
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					GameplayAbility,
					GetCurrentLevel(),
					static_cast<int32>(GameplayAbility.GetDefaultObject()->InputID),
					this));
	}
	for (TPair<FName, TSubclassOf<UXAbilityFlow>>& Kvp : DefaultFlows)
	{
		if (Kvp.Value)
			AbilitySystemComponent->GiveAbilityFlow(FAbilityFlowSpec(Kvp.Value, Kvp.Key));
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

void AXCharacterBase::ResetAllExecutionIndices() const
{
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->ResetAllExecutionIndices();
	}
}

void AXCharacterBase::GrantAbilityFromItem(UXItem* Item)
{
	FName Slot = "RightHand";
	if (AbilitySystemComponent)
	{
		if (Item->GrantedAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Item->GrantedAbility, GetCurrentLevel()));
					// Temporarily slotting into constant slot
			SlottedAbilities.Add(Slot);
		}
		else if (Item->GrantedAbilityFlow)
		{
			AbilitySystemComponent->GiveAbilityFlow(FAbilityFlowSpec(Item->GrantedAbilityFlow, Slot));
		}
	}
}

