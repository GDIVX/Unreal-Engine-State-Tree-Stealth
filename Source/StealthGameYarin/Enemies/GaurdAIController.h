// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardEnemy.h"
#include "StateTree.h"
#include "Components/StateTreeComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "GaurdAIController.generated.h"

class AGuardEnemy;
class UStateTree;
class UStateTreeComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class STEALTHGAMEYARIN_API AGaurdAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGaurdAIController();

	virtual void OnPossess(APawn* InPawn) override;

	FORCEINLINE UStateTreeComponent* GetStateTreeComponent() { return StateTreeComponent; }



private:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UStateTreeComponent> StateTreeComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UStateTree> StateTreeAsset;


	
	
};


