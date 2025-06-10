// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "weapon.h"
#include "SniperRifle.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ASniperRifle : public Aweapon
{
	GENERATED_BODY()

public:
	virtual void Fire() override;
	
};
