#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DetectionComponent.generated.h"

//Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWatchingTargetChanged, AActor*, Target);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHGAMEYARIN_API UDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public: 
	UDetectionComponent();

	UPROPERTY(BlueprintAssignable, Category = "Detection")
	FOnWatchingTargetChanged OnWatchingPawnStart;

	UPROPERTY(BlueprintAssignable, Category = "Detection")
	FOnWatchingTargetChanged OnWatchingPawnEnd;
	
	void StartWatching(AActor* TargetActor);
	void StopWatching();

	UFUNCTION(BlueprintPure, Category="Detection AI")
	FORCEINLINE AActor* GetCurrentTarget() const { return CurrentTarget; }

	UFUNCTION(BlueprintPure, Category="Detection AI")
	FORCEINLINE bool IsWatchingTarget() const { return CurrentTarget != nullptr; }

private:
	UPROPERTY(VisibleAnywhere, Category="Detection State")
	AActor* CurrentTarget;
};