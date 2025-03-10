// Fill out your copyright notice in the Description page of Project Settings.


#include "weapon.h"
#include "Components/SphereComponent.h"
#include "BlasterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "BulletShell.h"
#include "Bullet.h"

// Sets default values
Aweapon::Aweapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



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

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(SceneComponent);


}



// Called when the game starts or when spawned
void Aweapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	//绑定碰撞事件
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
	/*UE_LOG(LogTemp, Warning, TEXT("Overlap"));*/
	ABlasterCharacter* Character = Cast<ABlasterCharacter>(OtherActor);
	if (Character)
	{
		Character->overlappingweapon = this;
		ShowpickupWidget(true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetVisibility: Overlap")));
	}



}

void Aweapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));*/
	ABlasterCharacter* Character = Cast<ABlasterCharacter>(OtherActor);
	if (Character)
	{
		Character->overlappingweapon = nullptr;
		ShowpickupWidget(false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SetVisibility: EndOverlap")));
	}
}

// 其实射线的EDN应该是在屏幕的中心点 要不然人物动画会让武器乱动 直直的发射是不会在屏幕中心的
void Aweapon::Fire()
{
	WeaponMesh->PlayAnimation(WeaponFire, false);

	FVector Start = ArrowComponent->GetComponentLocation();   
	FVector End;

	//获取屏幕中心点
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// 获取摄像机的位置和旋转
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 计算摄像机正前方终点（屏幕中心对应的世界坐标方向）
		FVector CameraDirection = CameraRotation.Vector();
		End = CameraLocation + CameraDirection * 10000.0f; // 射线长度设为10000单位

		// 调整射线起点为武器位置，方向指向摄像机瞄准点
		FVector WeaponToEnd = End - Start;
		WeaponToEnd.Normalize();
		End = Start + WeaponToEnd * 10000.0f; // 保持射线长度一致

		 //设置武器的旋转，使其与射线方向一致
		FRotator WeaponRotation = WeaponToEnd.Rotation();
		SetActorRotation(WeaponRotation);


	}

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // 忽略武器自身
	CollisionParams.AddIgnoredActor(GetOwner()); // 忽略武器的拥有者
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	// 绘制射线
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

	if (bHit && HitResult.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));

		// 检查是否击中了 ABlasterCharacter
		ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			// 调用 PlayHitReactMontage
			HitCharacter->PlayHitReactMontage();
		}
	}

	// 生成弹壳
	if (WeaponMesh && BulletShellClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);

			UWorld* World = GetWorld();
			if (World)
			{
				ABulletShell* BulletShell = World->SpawnActor<ABulletShell>(BulletShellClass, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
				BulletShell->ApplyRandomForce();
			}
		}
	}
}


