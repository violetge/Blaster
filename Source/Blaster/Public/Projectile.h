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
	// ���÷�����
	void SetInstigatorActor(AActor* Instigator);

	// ��ȡ�˺�ֵ
	float GetDamage() const;

	// ��ը�߼�
	void Explode();

	// �����ڵ�
	void DestroyProjectile();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;

	//��ײ���
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	//�ƶ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// �˺�ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;


	// ��ը��Χ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float ExplosionRadius;

	// ��ը��Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ExplosionEffect;

	// ��ը��Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* ExplosionSound;

	// ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	AActor* InstigatorActor;

	// �Ƿ���Դ�͸
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//bool bCanPenetrate;

	// �ڵ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeSpan;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
