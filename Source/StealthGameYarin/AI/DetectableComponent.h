#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DetectableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeingWatchedStartSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeingWatchedAnySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotBeingWatchedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHiddenSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnwatchAnySignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class STEALTHGAMEYARIN_API UDetectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDetectableComponent();

	UPROPERTY(BlueprintAssignable, Category="Detection")
	FOnDetectedSignature OnDetected;

	UPROPERTY(BlueprintAssignable, Category="Detection")
	FOnBeingWatchedStartSignature OnBeingWatchedStart;

	UPROPERTY(BlueprintAssignable, Category="Detection")
	FOnBeingWatchedAnySignature OnBeingWatchedAny;

	UPROPERTY(BlueprintAssignable, Category="Detection")
	FOnNotBeingWatchedSignature OnNotBeingWatched;

	UPROPERTY(BlueprintAssignable, Category="Detection")
	FOnHiddenSignature OnHidden;

	UPROPERTY(BlueprintAssignable, Category="Detection")
	FOnUnwatchAnySignature OnUnwatchAny;


	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/** Return the current detection score. 0 = completely hidden. 1 = fully detected. */
	UFUNCTION(BlueprintPure, Category="Detection")
	FORCEINLINE float GetDetection() const { return DetectionScore; }

	/** Return TRUE if any actor with Detection component can see this actor */
	UFUNCTION(BlueprintPure, Category="Detection")
	FORCEINLINE bool IsBeingWatched() const { return WatcherCount > 0; }

	/** Return a trail of locations of where the actor have been in the last few frames.
	 * Useful for when a watcher lose line of sight but still want to track it. */
	UFUNCTION(BlueprintPure)
	TArray<FVector> GetBreadcrumbs() const { return Breadcrumbs; }
	

	// Watchers management
	void AddWatcher();
	void RemoveWatcher();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection")
	float DetectionGainRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection")
	float DetectionLoseRate;

	void DropBreadcrumb();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DetectionScore;

	void UpdateDetection(float DeltaTime);

	int32 WatcherCount;

	// Breadcrumbs

	// Max amount of breadcrumbs this pawn would drop.
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxBreadcrumbs = 5;

	// Distance between drops
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float DropDistanceThreshold = 200.0f;

	TArray<FVector> Breadcrumbs;
};
