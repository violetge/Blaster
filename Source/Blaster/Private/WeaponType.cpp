// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponType.h"

WeaponType::WeaponType()
{
}

WeaponType::~WeaponType()
{
}

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_None      UMETA(DisplayName = "None"),
    EWT_Pistol    UMETA(DisplayName = "Pistol"),
    EWT_Rifle     UMETA(DisplayName = "Rifle"),
    EWT_Sniper    UMETA(DisplayName = "Sniper"),
    EWT_Shotgun   UMETA(DisplayName = "Shotgun"),
    EWT_Rocket    UMETA(DisplayName = "Rocket"),
    EWT_MAX       UMETA(DisplayName = "DefaultMAX")
};