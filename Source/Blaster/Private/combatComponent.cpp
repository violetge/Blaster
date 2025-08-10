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
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);//����ZoomInterpSpeed�ǹ̶��� Ҳ������CurrentWeapon->GetZoomInterpSpeed() �������ݵ�ʱ�������ŵ�ʱ��һ��
		//̫���� ���Ե�����������  focal distance �� Aperture(F-stop)
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
	// ����Ƿ��������������Խ���
	if (CurrentWeapon == nullptr || SecondaryWeapon == nullptr) return;

	// ���ݵ�ǰ�����͸�����������
	Aweapon* TempWeapon = CurrentWeapon;

	// �ȷ�����������
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SecondaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// ������������
	CurrentWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;

	// ���¸���������
	CurrentWeapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		"WeaponSocket"
	);

	// ���¸��Ӹ�����
	SecondaryWeapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		"SecondaryWeaponSocket"
	);

	// ȷ���������Ǽ���״̬���������ǷǼ���״̬
	CurrentWeapon->SetActorEnableCollision(false);
	SecondaryWeapon->SetActorEnableCollision(false);

	// ȷ��ʰȡ�ؼ���ʾ״̬��ȷ
	CurrentWeapon->ShowpickupWidget(false);
	SecondaryWeapon->ShowpickupWidget(false);

	// ��������״̬��־
	IsWeaponEquipped = true;
}
