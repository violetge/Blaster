// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "weapon.h"
#include <Projectile.h>
#include "ProjectileWeapon.generated.h"

UCLASS()
class BLASTER_API AProjectileWeapon : public Aweapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 重写 Fire 函数
	virtual void Fire() override;

protected:
	// 抛射物类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

};
