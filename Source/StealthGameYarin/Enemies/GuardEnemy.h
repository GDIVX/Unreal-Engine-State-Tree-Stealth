// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GuardEnemy.generated.h"

class UDetectionComponent;
class APatrolPath;
class UWidgetComponent;
class UPawnSensingComponent;
class USplineComponent;

UCLASS()
class STEALTHGAMEYARIN_API AGuardEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuardEnemy();

	UFUNCTION(BlueprintCallable, Category = "AI")
	FVector GetNextPatrolPoint();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void Die() const;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Awarness")
	bool bSeePlayer;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "AI")
	TObjectPtr<APatrolPath> CurrentPatrolPath;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE void SetMaxMoveSpeed(const float InSpeed) const { GetCharacterMovement()->MaxWalkSpeed = InSpeed; }

	
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnPawnLostSight();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void ClearPatrolPath() { CurrentPatrolPath = nullptr; }

private:

	void FindNearestPatrolPath();


	//Private indexer for iteration on the patrol path
	int32 CurrentPatrolIndex = -1;

	// Detection

	UPROPERTY(VisibleAnywhere, Category = "AI" , meta=(AllowPrivateAccess="true"))
	TObjectPtr<UDetectionComponent> DetectionComponent;
	

	UPROPERTY(EditAnywhere, Category="AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	FTimerHandle TimerHandle_Sight;


};
