// Fill out your copyright notice in the Description page of Project Settings.

#include "SniperRifle.h"
#include "BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"


void ASniperRifle::Fire()
{
	if (CurrentAmmo > 0 && BCanFire)
	{
		WeaponMesh->PlayAnimation(WeaponFire, false);

		FVector Start = ArrowComponent->GetComponentLocation();
		FVector End;

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			// 没有弹道扩散，直接使用摄像机正前方
			FVector CameraDirection = CameraRotation.Vector();
			End = CameraLocation + CameraDirection * 10000.0f;

			FVector WeaponToEnd = End - Start;
			WeaponToEnd.Normalize();
			End = Start + WeaponToEnd * 10000.0f;

			FRotator WeaponRotation = WeaponToEnd.Rotation();
			SetActorRotation(WeaponRotation);
		}

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(GetOwner());
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1, 0, 1);

		if (bHit && HitResult.GetActor())
		{
			ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(HitResult.GetActor());
			if (HitCharacter && HitResult.Component == HitCharacter->GetMesh())
			{
				HitCharacter->PlayHitReactMontage();
				if (HitCharacter->HealthComponent)
				{
					HitCharacter->HealthComponent->DecreaseHealth(Damage);
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

