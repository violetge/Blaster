// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"  
#include "Components/CapsuleComponent.h"
#include "combatComponent.h"
#include "weapon.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include <BlasterPlayerController.h>
#include "GameFramework/PlayerStart.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/GameModeBase.h>

#include <Blaster/GameMode/BlasterModeBase.h>
#include <Grenade.h>

// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->bIsCrouched = false;



	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent,USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	CombatComponent = CreateDefaultSubobject<UcombatComponent>(TEXT("CombatComponent"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	
	// 初始化开火模式
	FireMode = EFireMode::Single;
}

void ABlasterCharacter::HandleDeath()
{
	// 打印角色死亡
	UE_LOG(LogTemp, Display, TEXT("HandleDeath"));

	PlayDeathtMontage();

	// 使角色无法移动
	//GetCharacterMovement()->DisableMovement();
	//GetCharacterMovement()->StopMovementImmediately();

	//重生的相关逻辑
	//ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(GetController());
	//if (PlayerController)
	//{
	//	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ABlasterCharacter::Respawn, 2, false);
	//}
	//else
	//{
	//	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ABlasterCharacter::Respawn, 2, false);
	//	UE_LOG(LogTemp, Error, TEXT("PlayerController is invalid!"));
	//}
}



//
//void ABlasterCharacter::Respawn()
//{
//	ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(GetController());
//	AActor* ChosenPlayerStart = ChooseRandomPlayerStart();
//	if (ChosenPlayerStart)
//	{
//		if (PlayerController)
//		{
//			PlayerController->UnPossess();
//		}
//		Destroy();
//
//		ABlasterModeBase* GameMode = GetWorld()->GetAuthGameMode<ABlasterModeBase>();
//		if (GameMode)
//		{
//			if (PlayerController)
//			{
//				GameMode->RestartPlayerAtPlayerStart(PlayerController, ChosenPlayerStart);
//			}
//			else
//			{
//				// 如果没有玩家控制器，直接生成新的敌人角色
//				FActorSpawnParameters SpawnParams;
//				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//				ABlasterCharacter* NewEnemy = GetWorld()->SpawnActor<ABlasterCharacter>(GetClass(), ChosenPlayerStart->GetActorLocation(), ChosenPlayerStart->GetActorRotation(), SpawnParams);
//				UE_LOG(LogTemp, Display, TEXT("RespawnEnemyCharacter"));
//			}
//		}
//		// 打印重生
//		UE_LOG(LogTemp, Display, TEXT("RespawnCharacter"));
//	}
//}
//
//AActor* ABlasterCharacter::ChooseRandomPlayerStart()
//{
//	UWorld* World = GetWorld();
//	if (World == nullptr) return nullptr;
//
//	// 获取所有的PlayerStart
//	TArray<AActor*> PlayerStarts;
//	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), PlayerStarts);
//
//	if (PlayerStarts.Num() > 0)
//	{
//		int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
//		// 打印重生点
//		UE_LOG(LogTemp, Display, TEXT("PlayerStarts: %s"), *PlayerStarts[RandomIndex]->GetName());
//		return PlayerStarts[RandomIndex];
//	}
//
//	return nullptr;
//}


UCameraComponent* ABlasterCharacter::GetCameraComponent() const
{
	return CameraComponent;
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	

}



// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	UpdateWeaponGripTransform();


	// 只有在武器装备时才实时更新武器的旋转
	if (CombatComponent && CombatComponent->IsWeaponEquipped)
	{
		UpdateWeaponRotation();
	}

}

float ABlasterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 检查健康组件是否存在
	if (HealthComponent)
	{
		// 调用健康组件的 DecreaseHealth 方法
		HealthComponent->DecreaseHealth(ActualDamage);
	}

	return ActualDamage;
}

// Called to bind functionality to input
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent * EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Move
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ABlasterCharacter::Action_MoveForward);
		EnhancedInputComponent->BindAction(IA_RightMove, ETriggerEvent::Triggered, this, &ABlasterCharacter::Action_RightMove);

		//Look
		EnhancedInputComponent->BindAction(IA_ControllerPitch, ETriggerEvent::Triggered, this, &ABlasterCharacter::Action_ControllerPitch);
		EnhancedInputComponent->BindAction(IA_ControllerYaw, ETriggerEvent::Triggered, this, &ABlasterCharacter::Action_ControllerYaw);

		//Pickup/drop
		EnhancedInputComponent->BindAction(PickUpWeapon, ETriggerEvent::Started, this, &ABlasterCharacter::AttachWeapon);

		//crouch
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &ABlasterCharacter::Action_Crouch);

		//Aim
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Started, this, &ABlasterCharacter::Aim_Pressed);
		EnhancedInputComponent->BindAction(IA_Aim, ETriggerEvent::Completed, this, &ABlasterCharacter::Aim_Released);

		//Fire
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ABlasterCharacter::Fire_Pressed);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ABlasterCharacter::Fire_Released);

		// Toggle fire mode
		EnhancedInputComponent->BindAction(IA_ToggleFireMode, ETriggerEvent::Started, this, &ABlasterCharacter::ToggleFireMode);

		// Reload weapon
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &ABlasterCharacter::ReloadWeapon);

		//Throw Grenade
		EnhancedInputComponent->BindAction(IA_ThrowGrenade, ETriggerEvent::Started, this, &ABlasterCharacter::ThrowGrenade);
	}

}

void ABlasterCharacter::UpdateWeaponGripTransform()
{
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon)
	{
		WeaponGripTransform = CombatComponent->CurrentWeapon->GetGripSocketTransform();
	}
}

void ABlasterCharacter::Action_MoveForward(const FInputActionValue& Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, Value.Get<float>());
}

void ABlasterCharacter::Action_RightMove(const FInputActionValue& Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightDirection, Value.Get<float>());
}

void ABlasterCharacter::Action_ControllerPitch(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>());
}

void ABlasterCharacter::Action_ControllerYaw(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}

void ABlasterCharacter::AttachWeapon()
{
	if (CombatComponent->IsWeaponEquipped)
	{
		CombatComponent->UnequipWeapon();
	}
	else 
	{

		CombatComponent->EquipWeapon(overlappingweapon);
	}
	UE_LOG(LogTemp, Display, TEXT("Started"));
}



void ABlasterCharacter::Action_Crouch()
{
	//别忘记加这个 蓝图里设置也可以 不然bIsCrouched不会变的哦
	/*GetCharacterMovement()->NavAgentProps.bCanCrouch = true;*/
	if (this->bIsCrouched)
	{
		UnCrouch();
		bIsCrouched = false;
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("this->bIsCrouched = false"));
	}
	else
	{
		Crouch();
		bIsCrouched = true;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		//更改胶囊体大小
		GetCapsuleComponent()->SetCapsuleHalfHeight(60.0f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("this->bIsCrouched = ture"));

	}
}

void ABlasterCharacter::Aim_Pressed()
{
	CombatComponent->IsAiming = true;
	GetCharacterMovement()->SetJumpAllowed(false);
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CombatComponent->IsAiming = true;"));
}

void ABlasterCharacter::Aim_Released()
{
	CombatComponent->IsAiming = false;
	GetCharacterMovement()->SetJumpAllowed(true);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CombatComponent->IsAiming = false;"));
}

void ABlasterCharacter::ToggleFireMode()
{
	if (FireMode == EFireMode::Single)
	{
		FireMode = EFireMode::Auto;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fire Mode: Auto"));
	}
	else
	{
		FireMode = EFireMode::Single;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Fire Mode: Single"));
	}
}

void ABlasterCharacter::ThrowGrenade()
{

	PlayThrowGrenadeMontage();

	SpawnAndThrowGrenade();
}


void ABlasterCharacter::ReloadWeapon()
{
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon && CombatComponent->CurrentWeapon->BCanFire && CombatComponent->CurrentWeapon->BackupAmmo > 0)
	{
		PlayReloadMontage();
		int32 AmmoNeeded = CombatComponent->CurrentWeapon->MaxAmmo - CombatComponent->CurrentWeapon->CurrentAmmo;
		int32 AmmoToReload = FMath::Min(AmmoNeeded, CombatComponent->CurrentWeapon->BackupAmmo);
		if (AmmoToReload > 0)
		{
			CombatComponent->CurrentWeapon->Reload(AmmoToReload);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No weapon equipped!"));
	}
}

void ABlasterCharacter::SpawnAndThrowGrenade()
{
	AGrenade* Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass,GetActorLocation(),GetActorRotation());
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->OwnerCharacter = this;

	}
}

void ABlasterCharacter::Fire_Pressed()
{
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon)
	{
		IsFire = true;
		if (FireMode == EFireMode::Single)
		{
			HandleFire();
		}
		else if (FireMode == EFireMode::Auto)
		{
			StartFire();
		}
	}
}

void ABlasterCharacter::Fire_Released()
{
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon)
	{
		IsFire = false;
		if (FireMode == EFireMode::Auto)
		{
			StopFire();
		}
	}
}



void ABlasterCharacter::StartFire()
{
	HandleFire();
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon)
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ABlasterCharacter::HandleFire, CombatComponent->CurrentWeapon->FireRate, true);
	}
}

void ABlasterCharacter::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void ABlasterCharacter::HandleFire()
{
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon && CombatComponent->CurrentWeapon->BCanFire)
	{
		PlayFireMontage();
		CombatComponent->CurrentWeapon->Fire();
	}
}

// 实现获取射线方向的函数
FRotator ABlasterCharacter::GetAimDirection() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		return CameraRotation;
	}
	return FRotator::ZeroRotator;
}

void ABlasterCharacter::UpdateWeaponRotation()
{
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon)
	{
		FRotator AimDirection = GetAimDirection();
		CombatComponent->CurrentWeapon->SetActorRotation(AimDirection);
	}
}
