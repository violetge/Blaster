// Fill out your copyright notice in the Description page of Project Settings.


#include "combatComponent.h"
#include "weapon.h"
#include "BlasterCharacter.h"

// Sets default values for this component's properties
UcombatComponent::UcombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	IsWeaponEquipped = false;
	IsAiming = false;

	// ...
}

void UcombatComponent::EquipWeapon(Aweapon* Weapon)
{
	if (Weapon)
	{
		CurrentWeapon = Weapon;
		CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
		Weapon->SetOwner(OwnerCharacter);
		IsWeaponEquipped = true;
		Weapon->ShowpickupWidget(false);
		CurrentWeapon->SetActorEnableCollision(false);
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


// Called when the game starts
void UcombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UcombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...


}
