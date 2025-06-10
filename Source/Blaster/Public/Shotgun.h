// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AShotgun : public Aweapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShotgun();

	virtual void Fire() override;

protected:
	// É¢µ¯Ç¹µ¯ÍèÊýÁ¿
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 PelletCount;

	// É¢µ¯Ç¹É¢Éä½Ç¶È£¨¶È£©
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float SpreadAngle;
	
};
