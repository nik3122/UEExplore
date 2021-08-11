// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "Abilities/XAttributeSet.h"
#include "GameFramework/Character.h"
#include "Abilities/XAbilitySystemComponent.h"
#include "Abilities/XGameplayAbility.h"
#include "GameplayEffect.h"
#include "XTypes.h"
#include "Items/Xitem.h"
#include "Items/XWeapon.h"

#include "XCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotItemChanged,  FXItemSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveSlotChanged, FXItemSlot, ActiveSlot);

DECLARE_LOG_CATEGORY_EXTERN(LogXCharacter, Log, All);

UCLASS()
class EXPLORE_API AXCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXCharacterBase();
	
	public:
	UPROPERTY(BlueprintAssignable)
	FOnActiveSlotChanged ActiveSlotChanged;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChanged SlotItemChanged;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable)
	float GetStamina() const;
	
	UFUNCTION(BlueprintCallable)
	AActor* GetEquippedWeapon() const;

	void HandleDamage(float const Damage);

protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;

	// Implement IAbilitySystemInterface
	UFUNCTION(BlueprintCallable)
	virtual UXAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Implement IGameplayTagAssetInterface
	UFUNCTION(BlueprintCallable)
	virtual void GetOwnedGameplayTags (FGameplayTagContainer & TagContainer) const override;
    
    /** Pointer to currently equipped weapon */
	UPROPERTY(BlueprintReadOnly)
	AActor* EquippedWeapon;
    
	/** === Blueprint events for relevant Attribute changes === */ 
	UFUNCTION(BlueprintImplementableEvent)
	void OnPoiseBreak();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken(float Damage);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged();
	/** ======================================================== */
	
	/** The component used to handle ability system interactions */
	UPROPERTY()
	UXAbilitySystemComponent* AbilitySystemComponent;
	
	/** List of attributes modified by the ability system */
	UPROPERTY()
	UXAttributeSet* AttributeSet;
    
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Abilities)
    TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectSpecHandles;
    
	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UXGameplayAbility>> StartingAbilities;

	UPROPERTY()
	bool bAbilitiesInitialized;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();

	/** Apply the DefaultGameplayEffect set on this Character */
	virtual void InitializeAttributes();

	/** Get current Character level. Mainly used by GAS */
	static int32 GetCurrentLevel();

	UFUNCTION()
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UXGameplayAbility>> Abilities);
	
	/** Abilities that are currently granted to this Character because slotted */	
	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayAbilitySpecHandle> SlottedAbilities;
	
	/** Get abilities that this character can afford to activate */
	UFUNCTION(BlueprintCallable)
	void GetAffordableAbilities(TArray<UXGameplayAbility*>& MatchingAbilities) const;

	UFUNCTION(BlueprintCallable)
	void GetActiveAbilitiesByTag(const FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& ActiveAbilities) const;
	
	/** Get abilities that this character can afford to activate, by tag */
	UFUNCTION(BlueprintCallable)
	void GetAffordableAbilitiesByTag(FGameplayTagContainer TagContainer, TArray<UXGameplayAbility*>& MatchingAbilities) const;

	/* -- ATTRIBUTE CHANGE CALLBACKS -- **/
	
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle PoiseChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void PoiseChanged(const FOnAttributeChangeData& Data);

	/* -------------------------------------- */
	
	// --------------------------------------
	//	SLOTTING SYSTEM
	// --------------------------------------
	UPROPERTY(BlueprintReadWrite)
	TMap<FXItemSlot, UXItem*> SlottedItems;

	/* Check if Character has an ASC and the Item has some one or more abilities to grant.
	 * If so, it grants them **/
	UFUNCTION(BlueprintCallable)
	void TryGrantAbilityFromItem(UXItem* Item);
	
	/* Equipping a weapon means spawning and attaching the related Weapon Actor
	 * and granting the Weapon Abilities. **/
	UFUNCTION(BlueprintCallable)
	bool EquipWeaponFromItem(UXWeapon* Weapon);

	/* Spawn given Weapon Actor and attach it to default weapon socket **/
	UFUNCTION(BlueprintCallable)
	AActor* AttachWeaponActor(TSubclassOf<AActor> WeaponActorClass);

	/* Executes every time the ActiveSlot changes **/
	UFUNCTION()
	void OnActiveSlotChanged(FXItemSlot NewItemSlot);
	
	/* A copy of the current hash key for the TMap holding the currently active Item **/
	UPROPERTY()
	FXItemSlot ActiveSlot;
	
public:
	/* Map an existing Slot to an Item, only if the Slot exists in SlottedItems **/
	UFUNCTION(BlueprintCallable)
	bool SetSlottedItem(FXItemSlot InItemSlot, UXItem* Item);

	/* Return the current Active Slot, which can be used to activate its Item's abilities. **/
	UFUNCTION(BlueprintCallable)
	FXItemSlot GetActiveSlot();

	/* Set the current Active Slot to the requested one, provided it is an existing Key in SlottedItems. **/
	UFUNCTION(BlueprintCallable)
	bool SetActiveSlot(FXItemSlot ItemSlot);
	
	/* Activate Abilities in given Slot, if exists **/
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateSlot(FXItemSlot ItemSlot, bool bAllowRemoteActivation = true);

	/* Add or replace an existing Item in SlottedItems. Return false if all SlottedItems are taken, true otherwise. **/
	UFUNCTION(BlueprintCallable)
	bool AddToSlottedItems(UXItem* Item);
};
