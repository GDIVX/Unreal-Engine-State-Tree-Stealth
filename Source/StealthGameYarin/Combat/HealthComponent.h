#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent, AActor*, DeadActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedEvent, float, CurrentHealth, float, MaxHealth, AActor*,
                                               Instigator);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHurtEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STEALTHGAMEYARIN_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UPROPERTY(BlueprintAssignable, Category = "Health Events")
	FOnDeathEvent OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Health Events")
	FOnHealthChangedEvent OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health Events")
	FOnHurtEvent OnHurt;

	UPROPERTY(BlueprintAssignable, Category = "Health Events")
	FOnHealEvent OnHeal;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float CurrentHealth;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                         AController* InstigatedBy, AActor* DamageCauser);
};
