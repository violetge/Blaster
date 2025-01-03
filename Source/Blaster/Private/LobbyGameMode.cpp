// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

ALobbyGameMode::ALobbyGameMode()
{
	UE_LOG(LogTemp, Display, TEXT("!ALobbyGameMode"));
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 LocalNumPlayers = GameState.Get()->PlayerArray.Num();
	if (LocalNumPlayers == 2)
	{

		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Map/Map1?ilsten"));
		}
	}

}

void ALobbyGameMode::StartGame()
{
	UE_LOG(LogTemp, Display, TEXT("!StartGame"));
}
