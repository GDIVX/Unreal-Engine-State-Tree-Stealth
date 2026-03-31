// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

#include "Components/SplineComponent.h"

// Sets default values
APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Patrol Spline"));
	RootComponent = SplineComponent;
}
