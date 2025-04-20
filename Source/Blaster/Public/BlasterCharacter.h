// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h" 
#include "HealthComponent.h"
#include "BlasterCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UinPutMappingContext;
class UInputAction;
class UcombatComponent;
class UHealthComponent;
class Aweapon;
class UCharacterMovementComponent;
class UAnimMontage;

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	Single UMETA(DisplayName = "Single"),
	Auto UMETA(DisplayName = "Auto")
};

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlasterCharacter();

	void HandleDeath();
	//void Respawn();
	//AActor* ChooseRandomPlayerStart();
	//FTimerHandle RespawnTimerHandle;


	FORCEINLINE UCameraComponent* GetCameraComponent()const;
	//C++定义 蓝图实现
	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayFireMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunct ions")
	void PlayHitReactMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayDeathtMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayReloadMontage();


	// Add this declaration to the ABlasterCharacter class
	UCameraComponent* GetFollowCamera() const { return CameraComponent; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsFire;

	// 声明一个函数，用于获取射线方向
	UFUNCTION(BlueprintCallable, Category = "Combat")
	FRotator GetAimDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UpdateWeaponRotation();

	void ReloadWeapon();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Action_MoveForward(const FInputActionValue& Value);
	void Action_RightMove(const FInputActionValue& Value);
	void Action_ControllerPitch(const FInputActionValue& Value);
	void Action_ControllerYaw(const FInputActionValue& Value);
	void Action_Crouch();
	void Aim_Pressed();
	void Aim_Released();
	void AttachWeapon();
	void Fire_Pressed();
	void Fire_Released();
	void ToggleFireMode();

	void PostInitializeComponents() override;


private:
	// 计时器句柄
	FTimerHandle FireTimerHandle;

	// 开火模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	EFireMode FireMode;


	// 自动开火函数
	void StartFire();
	void StopFire();
	void HandleFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 重写 TakeDamage 函数
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateWeaponGripTransform();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = "Character")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UcombatComponent* CombatComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Aim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Fire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_RightMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_ControllerPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_ControllerYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PickUpWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_ToggleFireMode;

	//武器在weapon.cpp重叠函数中赋值
	Aweapon* overlappingweapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FTransform WeaponGripTransform;


};
