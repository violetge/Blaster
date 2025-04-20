// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include <BlasterCharacter.h>

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	 // set up a notification for when this component hits something blocking
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));


	// ƒ¨»œ Ù–‘
	Damage = 100.f;
	ExplosionRadius = 100.f;
	//bCanPenetrate = false;
	LifeSpan = 5.f;
}



// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectile::DestroyProjectile()
{
	Destroy();
}

void AProjectile::SetInstigatorActor(AActor* NewInstigator)
{
	InstigatorActor = NewInstigator;
}

float AProjectile::GetDamage() const
{
	return Damage;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();



	DestroyProjectile();
}





void AProjectile::Explode() 
{
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}

	// ”¶”√±¨’®∑∂Œß…À∫¶
	UGameplayStatics::ApplyRadialDamage(
		this,                           // Damage Causer
		Damage,                         // Base Damage
		GetActorLocation(),             // Origin of the explosion
		ExplosionRadius,                // Radius of the explosion
		nullptr,                        // Damage type (can be nullptr for default)
		TArray<AActor*>(),              // Actors to ignore
		this,                           // Damage causer (this projectile)
		GetInstigatorController(),      // Instigator controller
		true                            // Do full damage
	);
}