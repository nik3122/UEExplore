// Fill out your copyright notice in the Description page of Project Settings.


#include "XCharacterBase.h"

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

UXAbilityFlow* AXCharacterBase::
CreateAbilityFlowInstance(FName Name)
{
	UE_LOG(LogTemp, Warning, TEXT("CREATING FLOW"));
	TSubclassOf<UXAbilityFlow>* FlowClass = AbilityFlowMap.Find(Name);
	if(!FlowClass)
	{
		return nullptr;	
	}
	UXAbilityFlow* NewFlow = NewObject<UXAbilityFlow>(this, FlowClass->GetDefaultObject()->GetClass());
	ActiveAbilityFlows.Emplace(Name, FAbilityFlowHandle(1, NewFlow));
	return NewFlow;
}

UXAbilityFlow* AXCharacterBase::GetAbilityFlowInstance(const FName Name)
{
	FAbilityFlowHandle* Handle = ActiveAbilityFlows.Find(Name);
	if (Handle)
	{
		return Handle->AbilityFlow;
	}
	return nullptr;
}

bool AXCharacterBase::TickExecuteAbilityFlow(const FName Name)
{
	UXAbilityFlow* AbilityFlow = GetAbilityFlowInstance(Name);

	if (!AbilityFlow)
	{
		AbilityFlow = CreateAbilityFlowInstance(Name);
		if(!AbilityFlow)
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("[AXCharacterBase::TickExecuteAbilityFlow] Attempted to Execute a non existing AbilityFlow.")
			);
			return false;
		}
	}
	const TSubclassOf<UXGameplayAbility> Ability = AbilityFlow->GetAbilityAtCurrentIndex();

	if(!Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("aBILITY IS invalid! WHYYYY"));

		return false;
	}
	
	const bool AbilityActivated = AbilitySystemComponent->TryActivateAbilityByClass(Ability);
	if(AbilityActivated)
	{
		// We tick the execution flow only if the ability was executed.
		// The flow is considered interrupted otherwise and must reset to 0.
		AbilityFlow->TickExecutionIndex();

	} else
	{
		AbilityFlow->ResetExecutionIndex();
	}
	return AbilityActivated;
}

void AXCharacterBase::ResetAllExecutionIndices()
{
	for (TPair<FName, FAbilityFlowHandle>& Kvp : ActiveAbilityFlows)
	{
		const int32 ExecutionIndex = Kvp.Value.AbilityFlow->GetCurrentExecutionIndex();
		if (ExecutionIndex != 0)
		{
			Kvp.Value.AbilityFlow->ResetExecutionIndex();
		}
	}
}
