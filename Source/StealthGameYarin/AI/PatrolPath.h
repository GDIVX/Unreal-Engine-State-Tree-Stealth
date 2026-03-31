// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS()
class STEALTHGAMEYARIN_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	FORCEINLINE USplineComponent* GetSplineComponent() const { return SplineComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<class USplineComponent> SplineComponent;



};
