// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "WeaponBase.h" 
#include "Animation/AnimMontage.h" 
#include "weapon.generated.h"

class USceneComponent;
class UskeletalMeshComponent;
class USphereComponent;
class UWidgetComponent;
class UstaticMeshComponent;
class UAnimationAsset;

UCLASS()
class BLASTER_API Aweapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aweapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ShowpickupWidget(bool IsShow);	  // ��ʾ������ʰȡС����
	FTransform GetGripSocketTransform() const;

	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }

	virtual void Fire() override;
	FVector CurrentAimDirection; // �洢��ǰ֡����׼����

	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;
	// ������	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireRate = 0;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	UAnimationAsset* WeaponFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere)
	float ZoomedFOV;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "BulletShell Type")
	TSubclassOf<class ABulletShell> BulletShellClass;

	// ��ǰ�ӵ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;

	// ���ӵ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 BackupAmmo;

	// װ�ҩ
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void Reload(int32 AmmoAmount);


	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	bool BCanFire;

	void EjectBulletShell();
private:
	// ���ҩ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;



	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);


};
