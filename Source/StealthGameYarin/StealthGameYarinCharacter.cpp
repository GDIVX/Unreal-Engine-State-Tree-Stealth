// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthGameYarinCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StealthGameYarin.h"
#include "Combat/HealthComponent.h"
#include "Combat/WeaponController.h"

AStealthGameYarinCharacter::AStealthGameYarinCharacter()
{
	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 96.0f);

	// Character rotation setup 
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Mesh 
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// First Person Camera 
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f)); // eye height
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// FOV
	FirstPersonCameraComponent->FieldOfView = 90.0f;

	// Movement tweaks
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Health
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	// Weapon
	WeaponController = CreateDefaultSubobject<UWeaponController>(TEXT("Weapon Controller"));
}


void AStealthGameYarinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
		                                   &AStealthGameYarinCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &AStealthGameYarinCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &AStealthGameYarinCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &AStealthGameYarinCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &AStealthGameYarinCharacter::LookInput);

		// Aiming Down Sight
		EnhancedInputComponent->BindAction(AimDownSightAction, ETriggerEvent::Started, this,
		                                   &AStealthGameYarinCharacter::DoStartAim);

		EnhancedInputComponent->BindAction(AimDownSightAction, ETriggerEvent::Completed, this,
		                                   &AStealthGameYarinCharacter::DoStopAim);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this,
								   &AStealthGameYarinCharacter::DoFire);
	}
	else
	{
		UE_LOG(LogStealthGameYarin, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}


void AStealthGameYarinCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AStealthGameYarinCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void AStealthGameYarinCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AStealthGameYarinCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AStealthGameYarinCharacter::DoJumpStart()
{
	Jump();
}

void AStealthGameYarinCharacter::DoJumpEnd()
{
	StopJumping();
}

void AStealthGameYarinCharacter::DoFire()
{
	if (WeaponController)
	{
		WeaponController->HandleFire();
	}
}

void AStealthGameYarinCharacter::DoStartAim()
{
	if (!IsValid(WeaponController))
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponController INVALID"));
		return;
	}
	WeaponController->StartAiming();
}

void AStealthGameYarinCharacter::DoStopAim()
{
	if (!IsValid(WeaponController))
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponController INVALID"));
		return;
	}
	WeaponController->StopAiming();
}
