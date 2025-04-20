// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "combatComponent.generated.h"

class Aweapon;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UcombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UcombatComponent();
	friend class ABlasterCharacter;

	void EquipWeapon(Aweapon* Weapon);
	void UnequipWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsWeaponEquipped;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsAiming;

	float DefaultFOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ZoomedFOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ZoomInterpSpeed;

	float CurrentFOV;
	void InterpFOV(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//角色在ABlasterCharacter. cpp 中赋值
	ABlasterCharacter* OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Aweapon* CurrentWeapon;
};
