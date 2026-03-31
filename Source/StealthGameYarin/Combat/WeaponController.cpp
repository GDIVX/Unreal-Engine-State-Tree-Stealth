#include "WeaponController.h"
#include "WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UWeaponController::UWeaponController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (PlayerCamera)
	{
		DefaultFOV = PlayerCamera->FieldOfView;
	}
}

void UWeaponController::EquipWeapon(const TSubclassOf<AWeaponBase> WeaponClass)
{
	if (!WeaponClass) return;

	if (IsValid(EquippedWeapon))
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());

	EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, Params);
	if (!EquippedWeapon) return;

	// Snap to initial socket (hand)
	UpdateADSAttachment();
}

void UWeaponController::HandleFire() const
{
	if (!EquippedWeapon || !PlayerCamera) return;

	const APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner) return;

	AController* PC = PawnOwner->GetController();
	if (!PC) return;

	const FVector CameraLoc = PlayerCamera->GetComponentLocation();
	const FVector CameraDir = PlayerCamera->GetForwardVector();

	EquippedWeapon->ExecuteFire(CameraLoc, CameraDir, PC);

	if (OnFire.IsBound())
	{
		OnFire.Broadcast();
	}
}

void UWeaponController::StartAiming()
{
	bIsAiming = true;
	UpdateADSAttachment();

	if (OnAimStateChanged.IsBound())
	{
		OnAimStateChanged.Broadcast(true);
	}
}

void UWeaponController::StopAiming()
{
	bIsAiming = false;
	UpdateADSAttachment();

	if (OnAimStateChanged.IsBound())
	{
		OnAimStateChanged.Broadcast(false);
	}
}

void UWeaponController::UpdateADSAttachment() const
{
	if (!EquippedWeapon) return;

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		USkeletalMeshComponent* Mesh = Character->GetMesh();
		if (!Mesh) return;

		const FName TargetSocket = bIsAiming ? AimSocket : HandSocket;
		EquippedWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TargetSocket);
	}
}

void UWeaponController::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerCamera) return;

	const float TargetFOV = bIsAiming ? AimingFOV : DefaultFOV;
	PlayerCamera->SetFieldOfView(FMath::FInterpTo(PlayerCamera->FieldOfView, TargetFOV, DeltaTime, InterpSpeed));

	if (IsValid(EquippedWeapon) && EquippedWeapon->GetWeaponMesh())
	{
		HandIKTargetLocation = EquippedWeapon->GetWeaponMesh()->GetSocketLocation(EquippedWeapon->GripSocketName);
	}
}