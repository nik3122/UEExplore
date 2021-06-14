#include "XCharacterBase.h"

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

	if (AbilitySystemComponent)
	{
		InitializeAttributes();
		AddStartupGameplayAbilities();
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AXCharacterBase::HealthChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AXCharacterBase::StaminaChanged);
		PoiseChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetPoiseAttribute()).AddUObject(this, &AXCharacterBase::PoiseChanged);
	}
}

void AXCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// Adding abilities here somehow causes them to activate twice
		// (which screws up combos). I'll figure out why, one day. Maybe.

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

auto AXCharacterBase::GetCurrentLevel() -> int32
{
	return 1;
}

float AXCharacterBase::GetHealth() const
{
	if (!AttributeSet)
		return 1.f;
	return AttributeSet->GetHealth();
}

float AXCharacterBase::GetMaxHealth() const
{
	if (!AttributeSet)
		return 1.f;
	return AttributeSet->GetMaxHealth();
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
		AbilitySystemComponent->BindAbilityActivationToInputComponent(
		PlayerInputComponent,
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
	if (!bAbilitiesInitialized)
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
		bAbilitiesInitialized = true;
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

void AXCharacterBase::GetActiveAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& ActiveAbilities) const
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesByTag(TagContainer, ActiveAbilities);
	}
}

void AXCharacterBase::GetAffordableAbilitiesByTag(const FGameplayTagContainer TagContainer,
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

void AXCharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (bAbilitiesInitialized)
		OnHealthChanged();
}

void AXCharacterBase::StaminaChanged(const FOnAttributeChangeData& Data)
{
}

void AXCharacterBase::PoiseChanged(const FOnAttributeChangeData& Data)
{
	const float Poise = Data.NewValue;

	if (Poise <= 0.0f)
	{
		// Notify Blueprint so it can handle it however necessary
		OnPoiseBreak();
	}
}

void AXCharacterBase::HandleDamage(const float Damage)
{
	if(bAbilitiesInitialized)
		OnDamageTaken(Damage);
}

void AXCharacterBase::GrantAbilityFromItem(UXItem* Item)
{
	if (AbilitySystemComponent)
	{
		for (TSubclassOf<UXGameplayAbility>& Ability : Item->GrantedAbilities)
		{
			if (Ability)
			{
				AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(
					Ability,
					GetCurrentLevel(),
					static_cast<int32>(Ability.GetDefaultObject()->InputID),
					this)
				);
			}
		}
	}
}

