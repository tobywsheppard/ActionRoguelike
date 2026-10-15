// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class UNiagaraComponent;
class UAudioComponent;
class URadialForceComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:

	ARogueExplosiveBarrel();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> BarrelMesh;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UNiagaraComponent> BurningEffectComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UAudioComponent> BurningSoundComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditAnywhere, Category="Explosion")
	float ExplosionDelay = 3.0f;

	FTimerHandle ExplodeTimerHandle;

	bool bExploded = false;

	void Explode();
};