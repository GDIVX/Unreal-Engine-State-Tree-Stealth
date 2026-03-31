// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameYarinGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnded, bool, bIsWin);

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AStealthGameYarinGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStealthGameYarinGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable , Category = "Gameplay")
	FOnGameEnded OnGameEnded;

	UFUNCTION(BlueprintCallable,Category="Gameplay")
	void EndGame( const bool bIsWin);

	UFUNCTION(BlueprintCallable ,Category="Gameplay")
	void SetPauseGame(bool bIsPaused) const;

	UFUNCTION(BlueprintImplementableEvent , Category="Gameplay")
	void ReciveGameEnd(const bool bIsWin);
};





