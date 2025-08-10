// Fill out your copyright notice in the Description page of Project Settings.

#include "combatComponent.h"
#include "weapon.h"
#include "BlasterCharacter.h"
#include "WeaponType.h"
#include "Camera/CameraComponent.h" // Add this include to resolve the incomplete type error

// Sets default values for this component's properties
UcombatComponent::UcombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	IsWeaponEquipped = false;
	IsAiming = false;

	ZoomedFOV = 45.0f;
	ZoomInterpSpeed = 20.0f;
	// ...
}

void UcombatComponent::EquipWeapon(Aweapon* Weapon)
{
	if (CurrentWeapon == nullptr && SecondaryWeapon == nullptr)
	{
		EquipPrimaryWeapon(Weapon);
	}
	else if (CurrentWeapon != nullptr && SecondaryWeapon == nullptr)
	{
		EquipSecondaryWeapon(Weapon);
	}
	else if(CurrentWeapon != nullptr && SecondaryWeapon != nullptr)
	{
		CurrentWeapon->SetActorEnableCollision(true);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->SetOwner(nullptr);
		CurrentWeapon->ShowpickupWidget(true);

		EquipPrimaryWeapon(Weapon);
	}

}

void UcombatComponent::UnequipWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorEnableCollision(true);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->SetOwner(nullptr);
		CurrentWeapon->ShowpickupWidget(true);
		CurrentWeapon = nullptr;
		IsWeaponEquipped = false;
	}
}


void UcombatComponent::InterpFOV(float DeltaTime)
{
	if (CurrentWeapon == nullptr) return;
	if (IsAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, CurrentWeapon->GetZoomedFOV(),DeltaTime, CurrentWeapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);//这里ZoomInterpSpeed是固定的 也可以是CurrentWeapon->GetZoomInterpSpeed() 这样回溯的时间与缩放的时间一致
		//太糊了 可以调整摄像机里的  focal distance 与 Aperture(F-stop)
	}
	OwnerCharacter->GetCameraComponent()->SetFieldOfView(CurrentFOV);
}

// Called when the game starts
void UcombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter->GetCameraComponent())
	{
		DefaultFOV = OwnerCharacter->GetCameraComponent()->FieldOfView;
		CurrentFOV = DefaultFOV;
	}
	
}


// Called every frame
void UcombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InterpFOV(DeltaTime);


}

void UcombatComponent::EquipPrimaryWeapon(Aweapon* Weapon)
{
	if (Weapon == nullptr) return;

	CurrentWeapon = Weapon;
	CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
	Weapon->SetOwner(OwnerCharacter);
	IsWeaponEquipped = true;
	Weapon->ShowpickupWidget(false);
	CurrentWeapon->SetActorEnableCollision(false);
}

void UcombatComponent::EquipSecondaryWeapon(Aweapon* Weapon)
{
	if (Weapon == nullptr) return;

	SecondaryWeapon = Weapon;
	SecondaryWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "SecondaryWeaponSocket");
	SecondaryWeapon->SetOwner(OwnerCharacter);
	SecondaryWeapon->ShowpickupWidget(false);
	SecondaryWeapon->SetActorEnableCollision(false);
}

void UcombatComponent::SwapWeapons()
{
	// 检查是否有两把武器可以交换
	if (CurrentWeapon == nullptr || SecondaryWeapon == nullptr) return;

	// 备份当前武器和副武器的引用
	Aweapon* TempWeapon = CurrentWeapon;

	// 先分离两把武器
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SecondaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 交换武器引用
	CurrentWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;

	// 重新附加主武器
	CurrentWeapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		"WeaponSocket"
	);

	// 重新附加副武器
	SecondaryWeapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		"SecondaryWeaponSocket"
	);

	// 确保主武器是激活状态，副武器是非激活状态
	CurrentWeapon->SetActorEnableCollision(false);
	SecondaryWeapon->SetActorEnableCollision(false);

	// 确保拾取控件显示状态正确
	CurrentWeapon->ShowpickupWidget(false);
	SecondaryWeapon->ShowpickupWidget(false);

	// 更新武器状态标志
	IsWeaponEquipped = true;
}
