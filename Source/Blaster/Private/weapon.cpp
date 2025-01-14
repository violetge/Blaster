// Fill out your copyright notice in the Description page of Project Settings.


#include "weapon.h"
#include "Components/SphereComponent.h"
#include "BlasterCharacter.h"
#include "Bullet.h"

// Sets default values
Aweapon::Aweapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;



	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(SceneComponent);

	StaticWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticWeaponMesh"));
	StaticWeaponMesh->SetupAttachment(SceneComponent);


	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(SceneComponent);


	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(SceneComponent);


}



// Called when the game starts or when spawned
void Aweapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	//°ó¶¨Åö×²ÊÂ¼þ
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &Aweapon::OnSphereOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &Aweapon::OnSphereEndOverlap);

	PickupWidget->SetVisibility(false);
	
}

// Called every frame
void Aweapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Aweapon::ShowpickupWidget(bool IsShow)
{
	PickupWidget->SetVisibility(IsShow);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetVisibility: %s"), IsShow ? TEXT("true") : TEXT("false")));
}

FTransform Aweapon::GetGripSocketTransform() const
{
	if (WeaponMesh)
	{
		return WeaponMesh->GetSocketTransform(FName("GripSocket"), ERelativeTransformSpace::RTS_World);
	}
	return FTransform::Identity;
}

void Aweapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	ABlasterCharacter* Character = Cast<ABlasterCharacter>(OtherActor);
	if (Character)
	{
		Character->overlappingweapon = this;
		ShowpickupWidget(true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetVisibility: Overlap")));
	}



}

void Aweapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
	ABlasterCharacter* Character = Cast<ABlasterCharacter>(OtherActor);
	if (Character)
	{
		Character->overlappingweapon = nullptr;
		ShowpickupWidget(false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetVisibility: EndOverlap")));
	}
}

void Aweapon::Fire()
{
	if (ProjectileClass)
	{
		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("MuzzleFlash"));
		FRotator MuzzleRotation = WeaponMesh->GetSocketRotation(FName("MuzzleFlash"));

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle
			ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			//if (Projectile)
			//{
			//	// Set the projectile's initial trajectory
			//	FVector LaunchDirection = MuzzleRotation.Vector();
			//	Projectile->FireInDirection(LaunchDirection);
			//}
		}
	}
}


