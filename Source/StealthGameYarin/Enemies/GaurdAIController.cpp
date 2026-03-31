// Fill out your copyright notice in the Description page of Project Settings.


#include "GaurdAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/StateTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AGaurdAIController::AGaurdAIController()
{
	bStartAILogicOnPossess = true;
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
	
}

void AGaurdAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn) return;



	if (StateTreeComponent && StateTreeAsset)
	{
		StateTreeComponent->SetStateTree(StateTreeAsset);
		StateTreeComponent->StartLogic();
	}
}
