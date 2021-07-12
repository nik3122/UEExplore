#include "Characters/XCharacterBase.h"

#include "XGameModeBase.h"

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

AActor* AXCharacterBase::SpawnAttachWeaponActor(const TSubclassOf<AActor> WeaponActorClass)
{
	const FVector Loc = FVector(0,0,1000);
	const FRotator Rotator = FRotator(0,0,0);
	AActor * SpawnedWeapon = GetWorld()->SpawnActor<AActor>(
			WeaponActorClass,
			Loc,
			Rotator,
			FActorSpawnParameters());
	if (SpawnedWeapon)
	{
		// Instigator is required to avoid colliding with self.
		SpawnedWeapon->SetInstigator(this);
		SpawnedWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false),
			FName("hand_rWeapon"));
		return SpawnedWeapon;
	}
	return nullptr;
}

void AXCharacterBase::AddSlotItem(UXItem* Item, FXItemSlot InItemSlot)
{
	if (Item)
	{
		SlottedItems.Emplace(InItemSlot, Item);
	}
}

FXItemSlot AXCharacterBase::GetActiveSlot()
{
	return ActiveSlot;
}

void AXCharacterBase::SetActiveSlot(const FXItemSlot Slot)
{
	ActiveSlot = Slot;
}

bool AXCharacterBase::EquipWeaponFromItem(UXWeapon* Weapon)
{
	if (Weapon)
	{
		const TSubclassOf<AActor> WeaponActorClass = Weapon->ActorClass.LoadSynchronous();
		if(IsValid(WeaponActorClass))
		{
			SpawnAttachWeaponActor(WeaponActorClass);
			GrantAbilityFromItem(Weapon);
			return true;
		}
	}
	return false;
}

bool AXCharacterBase::ActivateAbilitiesWithItemSlot(const FXItemSlot ItemSlot, const bool bAllowRemoteActivation)
{
	UXItem* Item = SlottedItems.FindRef(ItemSlot);

	// Weapon Abilities are activated by Input press once equipped.
	if (Item && Item->ItemType != UXAssetManager::WeaponItemType)
	{
		if (!Item->GrantedAbilities.IsEmpty() && AbilitySystemComponent)
		{
			for (auto Ability : Item->GrantedAbilities)
			{
				AbilitySystemComponent->TryActivateAbilityByClass(Ability, bAllowRemoteActivation);
			}
		}
	}
	return false;
}

bool AXCharacterBase::ActivateActiveItemSlot(bool bAllowRemoteActivation)
{
	return ActivateAbilitiesWithItemSlot(ActiveSlot, bAllowRemoteActivation);
}