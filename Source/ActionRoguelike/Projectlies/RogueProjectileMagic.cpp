// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ARogueProjectileMagic::ARogueProjectileMagic()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(16.0f);
	SphereComponent->SetCollisionProfileName("Projectile");
	
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComponent"));
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementCom"));
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ARogueProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileMagic::OnActorHit);
	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	// @todo: create our own damage type
	TSubclassOf<UDamageType> DmgTypeClass = UDamageType::StaticClass();
	
	UGameplayStatics::ApplyDamage(OtherActor, 10.f, GetInstigatorController(), this,DmgTypeClass);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	
	Destroy();
}