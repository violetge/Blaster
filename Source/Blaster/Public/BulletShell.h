// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletShell.generated.h"

class UStaticMeshComponent;
class USoundCue;

UCLASS()
class BLASTER_API ABulletShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletShell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ShellMesh;

	UPROPERTY(EditAnywhere)
	USoundCue* ShellSound;

	// 添加应用随机力的函数
	void ApplyRandomForce();
};
