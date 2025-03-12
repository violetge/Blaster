// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h" 
#include "BlasterCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UinPutMappingContext;
class UInputAction;
class UcombatComponent;
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

	FORCEINLINE UCameraComponent* GetCameraComponent()const;
	//C++���� ��ͼʵ��
	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayFireMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayHitReactMontage();


	// Add this declaration to the ABlasterCharacter class
	UCameraComponent* GetFollowCamera() const { return CameraComponent; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsFire;

	// ����һ�����������ڻ�ȡ���߷���
	UFUNCTION(BlueprintCallable, Category = "Combat")
	FRotator GetAimDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UpdateWeaponRotation();


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
	// ��ʱ�����
	FTimerHandle FireTimerHandle;

	// ����ģʽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	EFireMode FireMode;


	// �Զ�������
	void StartFire();
	void StopFire();
	void HandleFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateWeaponGripTransform();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = "Character")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UcombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Aim;

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

	//������weapon.cpp�ص������и�ֵ
	Aweapon* overlappingweapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FTransform WeaponGripTransform;


};
