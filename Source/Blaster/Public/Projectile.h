// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	// 设置发射者
	void SetInstigatorActor(AActor* Instigator);

	// 获取伤害值
	float GetDamage() const;

	// 爆炸逻辑
	void Explode();

	// 销毁炮弹
	void DestroyProjectile();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;

	//碰撞组件
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	//移动组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// 伤害值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;


	// 爆炸范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float ExplosionRadius;

	// 爆炸特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ExplosionEffect;

	// 爆炸音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* ExplosionSound;

	// 发射者
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	AActor* InstigatorActor;

	// 是否可以穿透
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//bool bCanPenetrate;

	// 炮弹寿命
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeSpan;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
