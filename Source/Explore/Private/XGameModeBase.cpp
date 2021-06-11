#include "XGameModeBase.h"

AXGameModeBase::AXGameModeBase()
{
}

void AXGameModeBase::GameOver()
{
	if (bGameOver == false)
	{
		K2_OnGameOver();
		bGameOver = true;
	}
}
