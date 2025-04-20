// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlasterPlayerController.h"
#include "BlasterModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	ABlasterModeBase();
	
//	// 处理角色死亡的函数
//	void HandleCharacterDeath(class ABlasterCharacter* BlasterCharacter);
//
//protected:
//
//	// 重生延迟时间
//	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
//	float RespawnDelay;
//
//	// 重生角色的函数
//	void RespawnCharacter(class ABlasterPlayerController* Controller);
//
//private:
//	// 定时器句柄
//	FTimerHandle RespawnTimerHandle;
//
//	// 选择一个随机的PlayerStart位置
//	AActor* ChooseRandomPlayerStart();
};
