// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "StealthGameYarinCharacter.generated.h"

class USpringArmComponent;
class UWeaponController;
class UHealthComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AStealthGameYarinCharacter : public ACharacter
{
	GENERATED_BODY()


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;


protected:
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> FireAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MouseLookAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> AimLookAction;

	/** Aim Down Sight Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> AimDownSightAction;

public:
	AStealthGameYarinCharacter();

protected:
	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Handles fire inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoFire();

	/** Handles start aiming down sight from controls */
	UFUNCTION(Blueprintable, Category="Input")
	virtual void DoStartAim();

	/** Handles start aiming down sight from controls */
	UFUNCTION(Blueprintable, Category="Input")
	virtual void DoStopAim();

protected:
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	// Components

	/** Manage the health state of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	TObjectPtr<UHealthComponent> HealthComponent;

	/** Manage the weapon wielded by the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UWeaponController> WeaponController;
};
