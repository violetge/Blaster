// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletShell.h"

// Sets default values
ABulletShell::ABulletShell()
{
	PrimaryActorTick.bCanEverTick = false;

	ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShellMesh"));
	RootComponent = ShellMesh;

	ShellSound = nullptr;  //UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation()); 这里是生成弹壳的声音的代码

}

// Called when the game starts or when spawned
void ABulletShell::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABulletShell::ApplyRandomForce()
{
	if (ShellMesh)
	{
		FVector RandomForce = FVector(FMath::RandRange(30.0f, 50.0f), FMath::RandRange(30.0f, 50.0f), FMath::RandRange(5.0f, 15.0f));
		ShellMesh->AddImpulse(RandomForce);
		//ShellMesh->AddImpulse(GetActorForwardVector()* 一个数（RandomForce）);  也可以
	}
}

