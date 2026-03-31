#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponController.generated.h"

class AWeaponBase;
class UCameraComponent;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimStateChanged, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFire);

/**
 * Handles weapon lifecycle (equip/fire) and ADS attachment switching.
 * 
 * Design:
 * - Weapon snaps between HandSocket and AimSocket
 * - Animation IK fixes hand placement; weapon itself moves via attachment
 * - Camera FOV is interpolated for visual feedback
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STEALTHGAMEYARIN_API UWeaponController : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponController();

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FOnAimStateChanged OnAimStateChanged;

	UPROPERTY(BlueprintAssignable, Category="Weapon|Events")
	FOnFire OnFire;

	/** Spawns and equips a weapon of the given class */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipWeapon(TSubclassOf<AWeaponBase> WeaponClass);

	/** Triggers the currently equipped weapon to fire using camera origin and direction */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void HandleFire() const;

	/** Enter ADS state */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StartAiming();

	/** Exit ADS state */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StopAiming();

	UFUNCTION(BlueprintPure, Category="Weapon")
	bool IsAiming() const { return bIsAiming; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	FVector HandIKTargetLocation;

protected:
	virtual void BeginPlay() override;

	/** Socket on character mesh where weapon attaches when not aiming */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName HandSocket = TEXT("HandGrip_R");

	/** Socket on character mesh for aiming */
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	FName AimSocket = TEXT("AimSocket");

	/** Target FOV while aiming (camera zoom) */
	UPROPERTY(EditDefaultsOnly, Category="Weapon|ADS")
	float AimingFOV = 60.f;

	/** Interpolation speed for FOV */
	UPROPERTY(EditDefaultsOnly, Category="Weapon|ADS")
	float InterpSpeed = 20.f;

private:
	/** Currently equipped weapon instance */
	UPROPERTY()
	TObjectPtr<AWeaponBase> EquippedWeapon;

	/** Cached player camera */
	UPROPERTY()
	TObjectPtr<UCameraComponent> PlayerCamera;

	/** Default camera FOV */
	float DefaultFOV = 90.f;

	/** Current aiming state */
	bool bIsAiming = false;

	/** Snap weapon to the correct socket based on ADS state */
	void UpdateADSAttachment() const;
};