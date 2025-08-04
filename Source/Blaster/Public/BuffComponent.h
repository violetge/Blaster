
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();
	class ABlasterCharacter* BlasterCharacter;
	class UHealthComponent* HealthComponent;

	void Heal(float HealAmount, float HealingTime);

protected:
	virtual void BeginPlay() override;

	void HealRampUp(float DeltaTime);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	//Health Buff

	bool bHealth = false;
	float HealthRate = 0.f;
	float AmountToHeal = 0.f;
};
