#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InvestigateTrailComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInvestigationComplete, FVector, FinalPredictedLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvestigationStarted);

UCLASS(ClassGroup=(AI), meta=(BlueprintSpawnableComponent))
class STEALTHGAMEYARIN_API UInvestigateTrailComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInvestigateTrailComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInvestigationComplete OnInvestigationComplete;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInvestigationStarted OnInvestigationStarted;

	UPROPERTY(EditAnywhere, Category = "Search")
	float ProjectionDistance = 500.f;

	UFUNCTION(BlueprintCallable, Category = "Search")
	void InitializeInvestigation(const TArray<FVector>& InTrail);

	UFUNCTION(BlueprintCallable, Category = "Search")
	FVector GetNextInvestigationPoint();

	UFUNCTION(BlueprintCallable, Category = "Search")
	FRotator GetInvestigativeLookAt() const;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Search")
	TArray<FVector> ActiveTrail;

	FVector LastConsumedPoint;
	FVector SecondLastConsumedPoint;

	UPROPERTY(BlueprintReadOnly, Category = "Search")
	bool bIsInvestigating = false;

	FVector ExtrapolateGhostPoint() const;
	bool IsPointVisible(const FVector& TargetPoint) const;
};
