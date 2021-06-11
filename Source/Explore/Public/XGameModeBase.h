#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XGameModeBase.generated.h"


UCLASS()
class EXPLORE_API AXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	
	AXGameModeBase();
	void GameOver();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category=Game)
	void OnGameOver();

	UPROPERTY(BlueprintReadOnly)
	bool bGameOver;
};
