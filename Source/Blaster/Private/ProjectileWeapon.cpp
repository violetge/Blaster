// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/ArrowComponent.h"
#include <BlasterCharacter.h>

// Sets default values
AProjectileWeapon::AProjectileWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileWeapon::Fire()
{
    if (CurrentAmmo > 0 && BCanFire && ProjectileClass)
    {
        // 获取武器的发射位置和方向
        FVector MuzzleLocation = ArrowComponent->GetComponentLocation();
        FRotator MuzzleRotation = ArrowComponent->GetComponentRotation();

        // 获取玩家控制器，用于获取瞄准方向
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            FVector CameraLocation;
            FRotator CameraRotation;
            PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

            // 调整发射方向为摄像机的瞄准方向
            MuzzleRotation = CameraRotation;
        }

        // 在世界中生成抛射物
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(
            ProjectileClass,
            MuzzleLocation,
            MuzzleRotation,
            SpawnParams
        );

        // 减少弹药数量
        CurrentAmmo--;
    }
    else if (CurrentAmmo <= 0)
    {
        // 自动换弹
        ABlasterCharacter* OwnerCharacter = Cast<ABlasterCharacter>(GetOwner());
        if (OwnerCharacter)
        {
            OwnerCharacter->ReloadWeapon();
        }
    }
}

