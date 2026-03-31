#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Mesh as root (cleaner hierarchy)
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void AWeaponBase::ExecuteFire(const FVector& CameraLocation, const FVector& ForwardVector,
                              AController* InstigatorController)
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (ForwardVector * WeaponRange);

	FHitResult HitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex=*/ true);

	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);

	if (bHit)
	{
		HandleHit(HitResult, InstigatorController);
	}

	HandleFireFeedback(TraceStart, ForwardVector, InstigatorController);
}

void AWeaponBase::HandleHit(const FHitResult& HitResult, AController* InstigatorController)
{
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor) return;

	UGameplayStatics::ApplyDamage(
		HitActor,
		BaseDamage,
		InstigatorController,
		this,
		UDamageType::StaticClass()
	);

	OnWeaponHit.Broadcast(HitResult.ImpactPoint, HitActor);
}

void AWeaponBase::HandleFireFeedback(const FVector& Origin, const FVector& Direction,
                                     AController* InstigatorController)
{
	OnWeaponFire.Broadcast(Origin, Direction, InstigatorController);
}
