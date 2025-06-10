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
	void ShowpickupWidget(bool IsShow);	  // 显示或隐藏拾取小部件
	FTransform GetGripSocketTransform() const;

	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }

	virtual void Fire() override;
	FVector CurrentAimDirection; // 存储当前帧的瞄准方向

	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;
	// 射击间隔	
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

	// 当前子弹数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;

	// 后备子弹数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 BackupAmmo;

	// 装填弹药
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void Reload(int32 AmmoAmount);


	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	bool BCanFire;

	void EjectBulletShell();
private:
	// 最大弹药容量
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
