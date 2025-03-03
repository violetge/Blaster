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
		PlayFireMontage();
		CombatComponent->CurrentWeapon->Fire();
	}
}

void ABlasterCharacter::Fire_Released()
{
	if (CombatComponent && CombatComponent->IsWeaponEquipped && CombatComponent->CurrentWeapon)
	{

	}
}



