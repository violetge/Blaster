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


	//����ײ�¼�
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

// ��ʵ���ߵ�EDNӦ��������Ļ�����ĵ� Ҫ��Ȼ���ﶯ�����������Ҷ� ֱֱ�ķ����ǲ�������Ļ���ĵ�
void Aweapon::Fire()
{
	WeaponMesh->PlayAnimation(WeaponFire, false);

	FVector Start = ArrowComponent->GetComponentLocation();   
	FVector End;

	//��ȡ��Ļ���ĵ�
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// ��ȡ�������λ�ú���ת
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// �����������ǰ���յ㣨��Ļ���Ķ�Ӧ���������귽��
		FVector CameraDirection = CameraRotation.Vector();
		End = CameraLocation + CameraDirection * 10000.0f; // ���߳�����Ϊ10000��λ

		// �����������Ϊ����λ�ã�����ָ���������׼��
		FVector WeaponToEnd = End - Start;
		WeaponToEnd.Normalize();
		End = Start + WeaponToEnd * 10000.0f; // �������߳���һ��

		 //������������ת��ʹ�������߷���һ��
		FRotator WeaponRotation = WeaponToEnd.Rotation();
		SetActorRotation(WeaponRotation);


	}

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // ������������
	CollisionParams.AddIgnoredActor(GetOwner()); // ����������ӵ����
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	// ��������
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

	if (bHit && HitResult.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));

		// ����Ƿ������ ABlasterCharacter
		ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			// ���� PlayHitReactMontage
			HitCharacter->PlayHitReactMontage();
		}
	}

	// ���ɵ���
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


