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
	
//	// �����ɫ�����ĺ���
//	void HandleCharacterDeath(class ABlasterCharacter* BlasterCharacter);
//
//protected:
//
//	// �����ӳ�ʱ��
//	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
//	float RespawnDelay;
//
//	// ������ɫ�ĺ���
//	void RespawnCharacter(class ABlasterPlayerController* Controller);
//
//private:
//	// ��ʱ�����
//	FTimerHandle RespawnTimerHandle;
//
//	// ѡ��һ�������PlayerStartλ��
//	AActor* ChooseRandomPlayerStart();
};
