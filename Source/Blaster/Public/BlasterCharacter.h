
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h" 
#include "HealthComponent.h"
#include "BuffComponent.h"
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
	FORCEINLINE UBuffComponent* GetBuffComponent() const { return BuffComponent; }	
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	//void Respawn();
	//AActor* ChooseRandomPlayerStart();
	//FTimerHandle RespawnTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int GrenadeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AGrenade>GrenadeClass;


	FORCEINLINE UCameraComponent* GetCameraComponent()const;
	//C++定义 蓝图实现
	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayFireMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayHitReactMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayDeathtMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayReloadMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "MyBlueprintFunctions")
	void PlayThrowGrenadeMontage();


	// Add this declaration to the ABlasterCharacter class
	UCameraComponent* GetFollowCamera() const { return CameraComponent; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool bIsProne = false;

	// 声明一个函数，用于获取射线方向
	UFUNCTION(BlueprintCallable, Category = "Combat")
	FRotator GetAimDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UpdateWeaponRotation();

	void ReloadWeapon();
	void SpawnAndThrowGrenade();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Action_MoveForward(const FInputActionValue& Value);
	void Action_RightMove(const FInputActionValue& Value);
	void Action_ControllerPitch(const FInputActionValue& Value);
	void Action_ControllerYaw(const FInputActionValue& Value);
	void Action_Crouch();
	void Action_Prone();
	void Aim_Pressed();
	void Aim_Released();
	void AttachWeapon();
	void Fire_Pressed();
	void Fire_Released();
	void ToggleFireMode();
	void ThrowGrenade();
	void SwapWeapons();

	void PostInitializeComponents() override;


private:

	// 蹲下时摄像机向前移动的距离
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CrouchCameraForwardOffset = 70.0f;

	// 摄像机移动插值速度
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CrouchCameraForwardInterpSpeed = 10.0f;

	// 趴下时摄像机向前移动的距离
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float ProneCameraForwardOffset = 150.0f;

	// 趴下时摄像机向下移动的距离
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float ProneCameraDownOffset = 30.0f;

	// 正在向前移动摄像机
	bool bIsCameraMovingForward = false;

	FVector OriginalSpringArmPosition;

	// 目标弹簧臂位置
	FVector TargetSpringArmPosition;

	// 当前弹簧臂位置
	FVector CurrentSpringArmPosition;

	// 处理摄像机向前移动
	void UpdateCameraForwardMovement(float DeltaTime);



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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBuffComponent* BuffComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* ThrowGrenadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Crouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Prone;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_ThrowGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_SwapWeapons;

	//武器在weapon.cpp重叠函数中赋值
	Aweapon* overlappingweapon;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FTransform WeaponGripTransform;


};
