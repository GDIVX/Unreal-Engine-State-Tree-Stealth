// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardEnemy.h"

#include "GaurdAIController.h"
#include "AI/DetectionComponent.h"
#include "AI/PatrolPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AGuardEnemy::AGuardEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Sensing
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	PawnSensingComponent->bSeePawns = true;
	PawnSensingComponent->SetPeripheralVisionAngle(30.0f);
	
	DetectionComponent = CreateDefaultSubobject<UDetectionComponent>(TEXT("Detection Mediator"));
}


// Called when the game starts or when spawned	
void AGuardEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Setup skeleton mesh
	if (USkeletalMeshComponent* MeshComponent = GetMesh())
	{
		MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}

	// Setup detection events
	// Sight :: when seeing the player - raise awareness and change state
	// We do this by using the bool bSeePlayer
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AGuardEnemy::OnPawnSeen);
	}

}

// Called every frame
void AGuardEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGuardEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGuardEnemy::FindNearestPatrolPath()
{
	TArray<AActor*> FoundPaths;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPath::StaticClass(), FoundPaths);

	float ClosestDistanceSq = UE_MAX_FLT;
	APatrolPath* BestPath = nullptr;

	// Find the closest path actor
	for (AActor* PathActor : FoundPaths)
	{
		if (const float DistSq = FVector::DistSquared(GetActorLocation(), PathActor->GetActorLocation()); DistSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistSq;
			BestPath = Cast<APatrolPath>(PathActor);
		}
	}

	CurrentPatrolPath = BestPath;

	// Find the closest point on that specific path to start from
	if (CurrentPatrolPath && CurrentPatrolPath->GetSplineComponent())
	{
		const USplineComponent* Spline = CurrentPatrolPath->GetSplineComponent();
		const int32 PointCount = Spline->GetNumberOfSplinePoints();

		float MinSplineDistSq = UE_MAX_FLT;
		int32 BestIndex = 0;

		for (int32 i = 0; i < PointCount; ++i)
		{
			const float DistToPoint = FVector::DistSquared(GetActorLocation(),
			                                         Spline->GetLocationAtSplinePoint(
				                                         i, ESplineCoordinateSpace::World));
			if (DistToPoint < MinSplineDistSq)
			{
				MinSplineDistSq = DistToPoint;
				BestIndex = i;
			}
		}
		CurrentPatrolIndex = BestIndex;
	}
}






void AGuardEnemy::OnPawnLostSight()
{
	bSeePlayer = false;
	DetectionComponent->StopWatching();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Sight);
}

void AGuardEnemy::OnPawnSeen(APawn* SeenPawn)
{
	if (!SeenPawn || !SeenPawn->ActorHasTag("Player"))
	{
		return;
	}
	bSeePlayer = true;
	DetectionComponent->StartWatching(SeenPawn);

	// Reset the timeout timer
	const float SightTimeout = PawnSensingComponent->SensingInterval + 0.1f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Sight, this, &AGuardEnemy::OnPawnLostSight, SightTimeout, false);
}

FVector AGuardEnemy::GetNextPatrolPoint()
{
	if (!CurrentPatrolPath)
	{
		FindNearestPatrolPath();
	}

	// Safety check
	if (!CurrentPatrolPath || !CurrentPatrolPath->GetSplineComponent())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed To Find a patrol path"));
		return GetActorLocation();
	}

	const USplineComponent* Spline = CurrentPatrolPath->GetSplineComponent();
	const int32 PointCount = Spline->GetNumberOfSplinePoints();

	if (PointCount == 0) return GetActorLocation();

	// Get the current target point, then increment the index for the next call
	const FVector TargetPoint = Spline->GetLocationAtSplinePoint(CurrentPatrolIndex, ESplineCoordinateSpace::World);
	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PointCount;

	return TargetPoint;
}


void AGuardEnemy::Die() const
{
	DetectionComponent->StopWatching();
}
