// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterModeBase.h"
#include "BlasterCharacter.h"
#include "BlasterPlayerController.h"
#include "GameFramework/PlayerStart.h" 
#include "Kismet/GameplayStatics.h"

ABlasterModeBase::ABlasterModeBase()
{
	UE_LOG(LogTemp, Display, TEXT("!ABlasterModeBase"));
}

//void ABlasterModeBase::HandleCharacterDeath(ABlasterCharacter* BlasterCharacter)
//{
//	//打印角色死亡
//	UE_LOG(LogTemp, Display, TEXT("HandleCharacterDeath"));
//
//	BlasterCharacter->PlayDeathtMontage();
//
//	ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//	if (PlayerController)
//	{
//		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateUObject(this, &ABlasterModeBase::RespawnCharacter, PlayerController), RespawnDelay, false);
//	}
//	else {
//		UE_LOG(LogTemp, Error, TEXT("PlayerController is invalid!"));
//	}
//}
//
//
//
//void ABlasterModeBase::RespawnCharacter(ABlasterPlayerController* Controller)
//{
//	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(Controller->GetPawn());
//
//	//if (BlasterCharacter)
//	//{
//	//	BlasterCharacter->Reset();
//	//	BlasterCharacter->Destroy();
//	//}
//	AActor* ChosenPlayerStart = ChooseRandomPlayerStart();
//	if (ChosenPlayerStart)
//	{
//		RestartPlayerAtPlayerStart(Controller, ChosenPlayerStart);
//		//打印重生
//		UE_LOG(LogTemp, Display, TEXT("RespawnCharacter"));
//	}
//}
//
//AActor* ABlasterModeBase::ChooseRandomPlayerStart()
//{
//	UWorld* World = GetWorld();
//	if (World == nullptr) return nullptr;
//
//	// 获取所有的PlayerStart
//	TArray<AActor*> PlayerStarts;
//	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);
//
//	if (PlayerStarts.Num() > 0)
//	{
//		int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
//		//打印重生点
//		UE_LOG(LogTemp, Display, TEXT("PlayerStarts: %s"), *PlayerStarts[RandomIndex]->GetName());
//		return PlayerStarts[RandomIndex];
//	}
//
//	return nullptr;
//}
