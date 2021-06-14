#include "XGameModeBase.h"

AXGameModeBase::AXGameModeBase()
{
}

void AXGameModeBase::GameOver()
{
	if (bGameOver == false)
	{
		bGameOver = true;
	}
}
