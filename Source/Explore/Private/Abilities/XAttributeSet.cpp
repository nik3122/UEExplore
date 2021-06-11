#include "Abilities/XAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "XCharacterBase.h"

void UXAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	AXCharacterBase* TargetCharacter = nullptr;
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor * TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter	= Cast<AXCharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (TargetCharacter)
		{
			TargetCharacter->HandleDamage(Data.EvaluatedData.Magnitude);
		}
	}
}
