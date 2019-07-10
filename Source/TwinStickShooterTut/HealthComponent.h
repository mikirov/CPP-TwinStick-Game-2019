// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChange, int, Health);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWINSTICKSHOOTERTUT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void TakeDamage(float Damage);

	void Heal(float HealingPower);

	float GetHealth();

	float GetMaxHealth();

	FDie OnDeath;

	FHealthChange OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly)
	int MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly)
	int Health;

	bool bDead = false;
};
