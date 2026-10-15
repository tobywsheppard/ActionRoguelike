// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

ARogueExplosiveBarrel::ARogueExplosiveBarrel()
{
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	RootComponent = BarrelMesh;
	BarrelMesh->SetSimulatePhysics(true);
	BarrelMesh->SetCollisionProfileName("PhysicsActor");
	
	BurningEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BurningEffectComponent"));
	BurningEffectComponent->SetupAttachment(BarrelMesh);
	BurningEffectComponent->SetAutoActivate(false);
	
	BurningSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BurningSoundComponent"));
	BurningSoundComponent->SetupAttachment(BarrelMesh);
	BurningSoundComponent->SetAutoActivate(false);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(BarrelMesh);
	RadialForceComponent->SetAutoActivate(false);
	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->ImpulseStrength = 2000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bIgnoreOwningActor = true;
}

float ARogueExplosiveBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (bExploded || GetWorldTimerManager().IsTimerActive(ExplodeTimerHandle))
	{
		return ActualDamage;
	}
	
	BurningEffectComponent->Activate();
	BurningSoundComponent->Activate();
	
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ARogueExplosiveBarrel::Explode, ExplosionDelay, false);
	
	return ActualDamage;
	
}

void ARogueExplosiveBarrel::Explode()
{
	if (bExploded)
	{
		return;
	}
	
	bExploded = true;
	
	BurningEffectComponent->Deactivate();
	BurningSoundComponent->Deactivate();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	RadialForceComponent->FireImpulse();
	
}