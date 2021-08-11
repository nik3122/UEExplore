#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "XAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class EXPLORE_API UXAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UXAttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UXAttributeSet, MaxHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UXAttributeSet, Stamina);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData MaxPoise;
	ATTRIBUTE_ACCESSORS(UXAttributeSet, MaxPoise);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	FGameplayAttributeData Poise;
	ATTRIBUTE_ACCESSORS(UXAttributeSet, Poise);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};

