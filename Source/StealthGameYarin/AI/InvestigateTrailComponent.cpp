#include "InvestigateTrailComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

UInvestigateTrailComponent::UInvestigateTrailComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsInvestigating = false;
	LastConsumedPoint = FVector::ZeroVector;
	SecondLastConsumedPoint = FVector::ZeroVector;
}

void UInvestigateTrailComponent::InitializeInvestigation(const TArray<FVector>& InTrail)
{
	if (InTrail.Num() == 0)
	{
		if (GetOwner())
		{
			OnInvestigationComplete.Broadcast(GetOwner()->GetActorLocation());
		}

		// Raise a warning - this is not a normal behaviour
		UE_LOG(LogTemp, Warning, TEXT("Investigation Started With An Empty Trail. Aborting."));
		return;
	}

	ActiveTrail = InTrail;
	bIsInvestigating = true;

	// Store the last two points for extrapolation later
	LastConsumedPoint = ActiveTrail.Last();
	SecondLastConsumedPoint = ActiveTrail.Num() > 1
		                          ? ActiveTrail[ActiveTrail.Num() - 2]
		                          : GetOwner()->GetActorLocation();

	OnInvestigationStarted.Broadcast();
}

FVector UInvestigateTrailComponent::GetNextInvestigationPoint()
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid GetNextInvestigationPoint Call. Owner Not Found.  "));
		return GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
	}

	if (!bIsInvestigating)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid GetNextInvestigationPoint Call. Not Currently Investigating. "));
		return GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
	}

	if (ActiveTrail.Num() == 0)
	{
		// Trail exhausted, calculate ghost point
		const FVector GhostPoint = ExtrapolateGhostPoint();

		bIsInvestigating = false;
		OnInvestigationComplete.Broadcast(GhostPoint);

		return GhostPoint;
	}

	// Pruning
	for (int32 i = ActiveTrail.Num() - 1; i >= 0; --i)
	{
		if (!IsPointVisible(ActiveTrail[i])) continue;

		// Remove all points up to and including the visible one
		const FVector TargetPoint = ActiveTrail[i];
		ActiveTrail.RemoveAt(0, i + 1, EAllowShrinking::Yes);

		return TargetPoint;
	}

	// If no points are visible, just walk to the immediate next one
	const FVector ImmediatePoint = ActiveTrail[0];
	ActiveTrail.RemoveAt(0, 1, EAllowShrinking::Yes);
	return ImmediatePoint;
}

FRotator UInvestigateTrailComponent::GetInvestigativeLookAt() const
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error,TEXT("Invalid GetInvestigativeLookAt Call. Owner Not Found."))
		return FRotator::ZeroRotator;
	}

	FVector LookTarget;

	if (ActiveTrail.Num() > 0)
	{
		// Look at the furthest known point in the trail
		LookTarget = ActiveTrail.Last();
	}
	else if (bIsInvestigating)
	{
		// Look toward the extrapolated direction
		LookTarget = ExtrapolateGhostPoint();
	}
	else
	{
		return GetOwner()->GetActorRotation();
	}

	return UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), LookTarget);
}

FVector UInvestigateTrailComponent::ExtrapolateGhostPoint() const
{
	const FVector Direction = (LastConsumedPoint - SecondLastConsumedPoint).GetSafeNormal();
	const FVector ProjectedPoint = LastConsumedPoint + (Direction * ProjectionDistance);

	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation NavLoc;
		if (NavSys->ProjectPointToNavigation(ProjectedPoint, NavLoc))
		{
			return NavLoc.Location;
		}
	}

	return ProjectedPoint;
}

bool UInvestigateTrailComponent::IsPointVisible(const FVector& TargetPoint) const
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return false;

	const FVector Eyes = OwnerPawn->GetPawnViewLocation();
	FHitResult Hit;

	const bool bBlocked = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Eyes,
		TargetPoint,
		ECC_Visibility
	);

	return !bBlocked || Hit.Location.Equals(TargetPoint, 50.f);
}
