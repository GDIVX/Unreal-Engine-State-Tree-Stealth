#include "DetectionComponent.h"
#include "DetectableComponent.h"

UDetectionComponent::UDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentTarget = nullptr;
}

void UDetectionComponent::StartWatching(AActor* TargetActor)
{
	if (!TargetActor || CurrentTarget == TargetActor) return;

	// If we switch targets immediately, stop watching the old one first
	if (CurrentTarget) StopWatching();

	CurrentTarget = TargetActor;

	if (UDetectableComponent* Detectable = CurrentTarget->GetComponentByClass<UDetectableComponent>())
	{
		Detectable->AddWatcher();
	}

	OnWatchingPawnStart.Broadcast(CurrentTarget);
}

void UDetectionComponent::StopWatching()
{
	if (!CurrentTarget)
	{
		return;
	}

	if (UDetectableComponent* Detectable = CurrentTarget->GetComponentByClass<UDetectableComponent>())
	{
		Detectable->RemoveWatcher();
	}
	
	OnWatchingPawnEnd.Broadcast(CurrentTarget);
	CurrentTarget = nullptr;
}
