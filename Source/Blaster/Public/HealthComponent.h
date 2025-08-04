// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 生命值变量
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health;

    // 最大生命值
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    // 减少生命值函数
    UFUNCTION(BlueprintCallable, Category = "Health")
    void DecreaseHealth(float Amount);

    // 增加生命值函数
    UFUNCTION(BlueprintCallable, Category = "Health")
    void IncreaseHealth(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealth() const;

    // 实现网络同步
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void SetHealth(float Amount) { Health = Amount; }
	float GetMaxHealth() const { return MaxHealth; }
};
