// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include <BlasterCharacter.h>
#include <Blaster/GameMode/BlasterModeBase.h>
#include <BlasterPlayerController.h>

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);

}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DecreaseHealth(float Amount)
{
    // ºı…Ÿ…˙√¸÷µ
    Health = FMath::Clamp(Health - Amount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Health after damage: %f"), Health);

    if (Health == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Health is zero, handling character death"));

        ABlasterCharacter* Character = Cast<ABlasterCharacter>(GetOwner());
        if (!Character)
        {
            UE_LOG(LogTemp, Error, TEXT("Character is invalid!"));
            return;
        }

        UWorld* World = GetWorld();
        if (World)
        {
           /* ABlasterModeBase* BlasterMode = Cast<ABlasterModeBase>(World->GetAuthGameMode());
            if (BlasterMode)
            {
                BlasterMode->HandleCharacterDeath(Character);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("BlasterModeBase not found!"));
            }*/
            if (Character)
            {
                Character->HandleDeath();
            }
        }
    }
}

void UHealthComponent::IncreaseHealth(float Amount)
{
	//
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
}
