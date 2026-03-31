#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnWeaponFire,
	FVector, Origin,
	FVector, Direction,
	AController*, InstigatorController
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWeaponHit,
	FVector, ImpactPoint,
	AActor*, HitActor
);

/**
 * Base class for all weapons.
 * 
 * Responsibilities:
 * - Executes firing logic (hitscan)
 * - Applies damage to hit actors
 * - Broadcasts events for VFX / audio / gameplay systems
 * 
 * Design:
 * - Camera-driven firing (FPS standard)
 * - Weapon is purely responsible for shot processing, not input
 */
UCLASS()
class STEALTHGAMEYARIN_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	/** Fired whenever the weapon performs a shot (even if nothing is hit) */
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FOnWeaponFire OnWeaponFire;

	/** Fired when a shot successfully hits an actor */
	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FOnWeaponHit OnWeaponHit;

	/** Name of the socket used for attaching the weapon to the character mesh */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName GripSocketName = TEXT("Grip");

	/**
	 * Executes a firing action using camera origin and direction.
	 * 
	 * @param CameraLocation        World location of the camera
	 * @param ForwardVector         Normalized forward direction of the camera
	 * @param InstigatorController  Controller responsible for the shot (used for damage attribution)
	 */
	virtual void ExecuteFire(
		const FVector& CameraLocation,
		const FVector& ForwardVector,
		AController* InstigatorController
	);

	/** Returns the weapon's mesh component */
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

protected:
	/** Visual representation of the weapon (root component) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	/** Base damage applied per successful hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Stats", meta=(ClampMin="0.0"))
	float BaseDamage = 20.0f;

	/** Maximum range of the weapon trace (in unreal units) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Stats", meta=(ClampMin="0.0"))
	float WeaponRange = 10000.0f;

	/** Processes a hit result, applies damage, and triggers OnWeaponHit */
	virtual void HandleHit(const FHitResult& HitResult, AController* InstigatorController);

	/** Broadcasts OnWeaponFire event, can be used to trigger VFX / audio / recoil */
	virtual void HandleFireFeedback(const FVector& Origin, const FVector& Direction,
									AController* InstigatorController);
};