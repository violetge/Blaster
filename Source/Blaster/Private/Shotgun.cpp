// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "BlasterCharacter.h"

AShotgun::AShotgun()
{
	PelletCount = 8;      // 默认8发弹丸
	SpreadAngle = 8.0f;   // 默认8度散射
}

void AShotgun::Fire()
{
	if (CurrentAmmo > 0 && BCanFire)
	{
		WeaponMesh->PlayAnimation(WeaponFire, false);

		FVector Start = ArrowComponent->GetComponentLocation();

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			for (int32 i = 0; i < PelletCount; ++i)
			{
				// 生成随机散射方向
				FRotator PelletRotation = CameraRotation;
				PelletRotation.Yaw += FMath::FRandRange(-SpreadAngle, SpreadAngle);
				PelletRotation.Pitch += FMath::FRandRange(-SpreadAngle, SpreadAngle);

				FVector End = Start + PelletRotation.Vector() * 10000.0f;

				FHitResult HitResult;
				FCollisionQueryParams CollisionParams;
				CollisionParams.AddIgnoredActor(this);
				CollisionParams.AddIgnoredActor(GetOwner());

				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

				DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 1, 0, 1);

				if (bHit && HitResult.GetActor())
				{
					ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(HitResult.GetActor());
					if (HitCharacter && HitResult.Component == HitCharacter->GetMesh())
					{
						HitCharacter->PlayHitReactMontage();
						if (HitCharacter->HealthComponent)
						{
							HitCharacter->HealthComponent->DecreaseHealth(Damage / PelletCount); // 每颗弹丸分摊伤害
						}
					}
				}
			}
		}

		EjectBulletShell();
		CurrentAmmo--;
	}
	else if (CurrentAmmo <= 0)
	{
		ABlasterCharacter* OwnerCharacter = Cast<ABlasterCharacter>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->ReloadWeapon();
		}
	}
}
