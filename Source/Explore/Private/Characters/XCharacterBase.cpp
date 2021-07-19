#include "Characters/XCharacterBase.h"

AXCharacterBase::AXCharacterBase()
{
	// Create default slots for this Character, with no Items.	
	for (int n = 0; n < 5; n++)
	{
		 SlottedItems.Add(FXItemSlot(n, UXAssetManager::WeaponItemType), nullptr);	
	}
	TArray<FXItemSlot> CreatedSlots;
	SlottedItems.GetKeys(CreatedSlots);

	if (!CreatedSlots.IsEmpty())
		ActiveSlot = CreatedSlots[0];

	// Delegate binding
	ActiveSlotChanged.AddDynamic(this, &AXCharacterBase::OnActiveSlotChanged);
	
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

AActor* AXCharacterBase::GetEquippedWeapon() const
{
	if (EquippedWeapon)
		return EquippedWeapon;
	return nullptr;
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

bool AXCharacterBase::TryGrantAbilityFromItem(UXItem* Item)
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
		return true;
	}
	return false;
}

AActor* AXCharacterBase::AttachWeaponActor(const TSubclassOf<AActor> WeaponActorClass)
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

bool AXCharacterBase::SetSlottedItem(FXItemSlot InItemSlot, UXItem* Item)
{
	if (Item)
	{
		for(auto Slot: SlottedItems)
		{
			if (Slot.Key == InItemSlot)
			{
				SlottedItems.Emplace(InItemSlot, Item);
				return true;
			}
		}
	}
	return false;
}

FXItemSlot AXCharacterBase::GetActiveSlot()
{
	return ActiveSlot;
}

bool AXCharacterBase::SetActiveSlot(const FXItemSlot ItemSlot)
{
	if (SlottedItems.Contains(ItemSlot))
	{
		ActiveSlot = ItemSlot;

		// BP Compatible Event
		ActiveSlotChanged.Broadcast(ItemSlot);
		return true;
	}
	return false;
}

bool AXCharacterBase::EquipWeaponFromItem(UXWeapon* Weapon)
{
	if (Weapon)
	{
		const TSubclassOf<AActor> WeaponActorClass = Weapon->ActorClass.LoadSynchronous();
		if(IsValid(WeaponActorClass))
		{
			EquippedWeapon = AttachWeaponActor(WeaponActorClass);
			return true;
		}
	}
	return false;
}

void AXCharacterBase::OnActiveSlotChanged(const FXItemSlot NewItemSlot)
{
	// Some Items are equippable and need to actually spawn in the World
	// upon slot activation, besides granting abilities.
	if (ActiveSlot.ItemType == UXAssetManager::WeaponItemType)
	{
		UXItem* Item = SlottedItems.FindRef(NewItemSlot);
		if (Item)
		{
			UXWeapon* Weapon = static_cast<UXWeapon*>(Item);
			EquipWeaponFromItem(Weapon);
			TryGrantAbilityFromItem(Weapon);
		}
	}
}

bool AXCharacterBase::ActivateSlot(const FXItemSlot ItemSlot, const bool bAllowRemoteActivation)
{
	UXItem* Item = SlottedItems.FindRef(ItemSlot);

	// Weapon Abilities are activated only by Input press once equipped.
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

bool AXCharacterBase::AddToSlottedItems(UXItem* Item)
{
	for (const auto Slot: SlottedItems)
	{
		if (Slot.Value == nullptr)
		{
			SlottedItems.Add(Slot.Key, Item);
			return true;
		}
	}

	return false;
	
}
