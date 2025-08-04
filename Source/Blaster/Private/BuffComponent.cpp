

#include "BuffComponent.h"
#include "BlasterCharacter.h"


UBuffComponent::UBuffComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealth = true;
	HealthRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;
}



void UBuffComponent::HealRampUp(float DeltaTime)
{

	if (!bHealth) return;
	BlasterCharacter = Cast<ABlasterCharacter>(GetOwner());

	HealthComponent = BlasterCharacter->GetHealthComponent();
	const float HealThisFrame = HealthRate * DeltaTime;
	HealthComponent->SetHealth(FMath::Clamp(HealthComponent->GetHealth() + HealThisFrame, 0.f, HealthComponent->GetMaxHealth()));
	AmountToHeal -= HealThisFrame;

	if (HealThisFrame <= 0 || HealthComponent->GetHealth() >= HealthComponent->GetMaxHealth())
	{
		bHealth = false;
		AmountToHeal = 0.f;
	}
}



void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealRampUp(DeltaTime);

	// ...
}

