#include "DetectableComponent.h"
#include "Math/UnrealMathUtility.h"

UDetectableComponent::UDetectableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	DetectionScore = 0.0f;
	WatcherCount = 0;
	DetectionGainRate = 0.2f;
	DetectionLoseRate = 0.04f;
}


void UDetectableComponent::UpdateDetection(const float DeltaTime)
{
	if (WatcherCount > 0 && DetectionScore < 1.0f)
	{
		DetectionScore = FMath::Clamp(DetectionScore + (DetectionGainRate * DeltaTime), 0.0f, 1.0f);
	}
	else if (WatcherCount == 0 && DetectionScore > 0.0f)
	{
		DetectionScore = FMath::Clamp(DetectionScore - (DetectionLoseRate * DeltaTime), 0.0f, 1.0f);
	}

	// If detection == 1 -> we have been discovered
	if (DetectionScore == 1.0f && OnDetected.IsBound())
	{
		OnDetected.Broadcast();
		return;
	}
	// If detection == 0 we are hidden
	if (DetectionScore == 0.0f && OnHidden.IsBound())
	{
		OnHidden.Broadcast();
		return;
	}
}

void UDetectableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateDetection(DeltaTime);

	DropBreadcrumb();
}


void UDetectableComponent::AddWatcher()
{
	WatcherCount++;

	// If watcher count is 1 -> we started being watched now
	if (WatcherCount == 1 && OnBeingWatchedStart.IsBound())
	{
		OnBeingWatchedStart.Broadcast();
	}

	if (OnBeingWatchedAny.IsBound())
	{
		OnBeingWatchedAny.Broadcast();
	}
}

void UDetectableComponent::RemoveWatcher()
{
	WatcherCount = FMath::Max(0, WatcherCount - 1);

	//if we are no longer being watched
	if (WatcherCount == 0 && OnNotBeingWatched.IsBound())
	{
		OnNotBeingWatched.Broadcast();
	}
	if (OnUnwatchAny.IsBound())
	{
		OnUnwatchAny.Broadcast();
	}
}

void UDetectableComponent::DropBreadcrumb()
{
	if (GetOwner()->GetVelocity().Size() > 10.0f)
	{
		const FVector CurrentLocation = GetOwner()->GetActorLocation();
		if (Breadcrumbs.IsEmpty() || FVector::Dist(CurrentLocation, Breadcrumbs.Last()) > DropDistanceThreshold)
		{
			Breadcrumbs.Add(CurrentLocation);
			if (Breadcrumbs.Num() > MaxBreadcrumbs)
			{
				Breadcrumbs.RemoveAt(0);
			}
		}
	}
}
