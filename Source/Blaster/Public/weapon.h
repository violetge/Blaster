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

	UPROPERTY(EditAnywhere, Category = "BulletShell Type")
	TSubclassOf<class ABulletShell> BulletShellClass;

	virtual void Fire() override;
	FVector CurrentAimDirection; // 存储当前帧的瞄准方向


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	UAnimationAsset* WeaponFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent;

private:

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
