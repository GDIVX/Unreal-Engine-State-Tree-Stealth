// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthGameYarinGameMode.h"

#include "Kismet/GameplayStatics.h"

AStealthGameYarinGameMode::AStealthGameYarinGameMode()
{
	// stub
}

void AStealthGameYarinGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetPauseGame(false);
}

void AStealthGameYarinGameMode::EndGame(const bool bIsWin)
{
	// Pause the game and switch to UI input
	SetPauseGame(true);
	ReciveGameEnd(bIsWin);

	if (OnGameEnded.IsBound())
	{
		OnGameEnded.Broadcast(bIsWin);
	}
}

void AStealthGameYarinGameMode::SetPauseGame(const bool bIsPaused) const
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetPause(bIsPaused);
		PlayerController->SetShowMouseCursor(bIsPaused);

		// Set input mode
		if (bIsPaused)
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
		else
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}
