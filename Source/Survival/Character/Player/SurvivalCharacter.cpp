// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Survival/SurvivalGameMode.h"
#include "Survival/UI/SurvivalHUD.h"
#include "Survival/UI/Menu/Inventory/InventoryMenu.h"
#include "Survival/Item/Weapon/Weapon.h"
#include "Survival/Character/AI/BaseAICharacter.h"

// Sets default values
ASurvivalCharacter::ASurvivalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), FName("spine_05"));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->bAutoActivate = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;
	ThirdPersonCamera->bAutoActivate = false;

	FirstPersonCamera->SetActive(true, false);

	FocusedEnemy = nullptr;
	
	AimPitch = 0.0f;
	FirstPerson = 950;
	ThirdPerson = 1300;
	
	WeaponComponent->SetupAttachment(GetMesh(), FName("HandGrip_R"));
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	ActivateCamera(FirstPersonCamera);

	ASurvivalGameMode* GameMode = GetWorld()->GetAuthGameMode<ASurvivalGameMode>();
	if (IsValid(GameMode->GetHUDInstance()))
	{
		OnHealthChanged.AddDynamic(GameMode->GetHUDInstance(), &USurvivalHUD::UpdatePlayerHealthBar);
		OnStaminaChanged.AddDynamic(GameMode->GetHUDInstance(), &USurvivalHUD::UpdatePlayerSprintBar);
		OnFocusedItem.AddDynamic(GameMode->GetHUDInstance(), &USurvivalHUD::UpdateItem);
		Inventory->OnItemAdded.AddDynamic(GameMode->GetHUDInstance(), &USurvivalHUD::UpdateInventory);
	}
}

void ASurvivalCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimPitch();
}

// Called to bind functionality to input
void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::Sprint);
		EnhancedInputComponent->BindAction(SwitchCameraAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::UpdateCamera);
		EnhancedInputComponent->BindAction(SheathWeaponAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::SheathWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASurvivalCharacter::Attack);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASurvivalCharacter::Interact);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ASurvivalCharacter::ShowInventory);
	}
}

void ASurvivalCharacter::RegenerateStamina()
{
	Super::RegenerateStamina();

	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

void ASurvivalCharacter::ToggleWeapon()
{
	Super::ToggleWeapon();
	
	UpdateCharacterMovementRotation();
}

void ASurvivalCharacter::Move(const FInputActionValue& Value)
{
	if (bCanMove)
	{
		// input is a Vector2D
		const FVector2D MovementVector = Value.Get<FVector2D>();
	
		if (IsValid(GetController()))
		{
			// find out which way is forward
			const FRotator Rotation = GetController()->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
		
			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
		UpdateFocus();
	}
}

void ASurvivalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(-LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
	UpdateFocus();
}

void ASurvivalCharacter::Sprint(const FInputActionValue& Value)
{
	SetIsSprinting(Value.Get<bool>());
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

void ASurvivalCharacter::UpdateCamera(const FInputActionValue& Value)
{
	const float ScrollValue = Value.Get<float>();
	if (ScrollValue > 0.f)
	{
		if (!FirstPersonCamera->IsActive() && CameraBoom->TargetArmLength <= 200.0f)
		{
			ActivateCamera(FirstPersonCamera);
			UpdateCharacterMovementRotation();
			UpdateFocus();
			return;
		}

		if (CameraBoom->TargetArmLength > 200.0f)
		{
			CameraBoom->TargetArmLength = CameraBoom->TargetArmLength -50.0f;
		}
	}
	else
	{
		if (!ThirdPersonCamera->IsActive())
		{
			ActivateCamera(ThirdPersonCamera);
			UpdateCharacterMovementRotation();
			UpdateFocus();
			return;
		}

		if (CameraBoom->TargetArmLength < 400.0f && !bIsWeaponReady)
		{
			CameraBoom->TargetArmLength = CameraBoom->TargetArmLength + 50.0f;
		}
	}
}

void ASurvivalCharacter::SheathWeapon(const FInputActionValue& Value)
{
	ToggleWeapon();
}

void ASurvivalCharacter::Attack(const FInputActionValue& Value)
{
	if (bIsAttacking || bIsFalling)
	{
		return;
	}
	
	if (!bIsWeaponReady)
	{
		ToggleWeapon();
		return;
	}
	if (IsValid(AttackMontage))
	{
		GetWeaponEquipped()->GetHitActorsThisAttack().Empty();
		bIsAttacking = true;
		SetIsSprinting(false);
		bCanMove = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(AttackMontage);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &ASurvivalCharacter::OnMeleeAttackMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	}
}

void ASurvivalCharacter::Interact(const FInputActionValue& Value)
{
	ABaseAICharacter* NewFocusedEnemy = nullptr;
	ABaseItem* NewFocusedItem = nullptr;
	float Distance;
	IsSomethingInFocusRange(NewFocusedEnemy, NewFocusedItem, Distance);
	if (IsValid(NewFocusedItem))
	{
		if ((FirstPersonCamera->IsActive() && Distance <= 150.0f) ||
			(ThirdPersonCamera->IsActive() && Distance <= 325.0f) ||
			(CameraBoom->TargetArmLength == 250 && Distance <= 375.0f) ||
			(CameraBoom->TargetArmLength == 300 && Distance <= 425.0f) ||
			(CameraBoom->TargetArmLength == 350 && Distance <= 475.0f) ||
			(CameraBoom->TargetArmLength == 400 && Distance <= 525.0f))
		{
			Inventory->AddItem(NewFocusedItem);
			UpdateFocus();
		}
	}
}

void ASurvivalCharacter::ShowInventory(const FInputActionValue& Value)
{
	ASurvivalGameMode* GameMode = GetWorld()->GetAuthGameMode<ASurvivalGameMode>();
	if (IsValid(GameMode->GetHUDInstance()))
	{
		GameMode->GetHUDInstance()->GetInventoryMenu()->OnInventoryItemSelected.AddDynamic(Inventory, &UInventory::NotifyItemSelected);
		Inventory->OnItemData.AddDynamic(GameMode->GetHUDInstance()->GetInventoryMenu(), &UInventoryMenu::ShowItemDetails);
		GameMode->GetHUDInstance()->HandleInventory(Inventory);
	}
}

void ASurvivalCharacter::ActivateCamera(UCameraComponent* Camera)
{
	FirstPersonCamera->SetActive(false, false);
	ThirdPersonCamera->SetActive(false, false);
	Camera->SetActive(true, false);
	if (FirstPersonCamera->IsActive())
	{
		GetMesh()->HideBoneByName(TEXT("Neck_01"), EPhysBodyOp::PBO_None);
		bUseControllerRotationYaw = true;
	}
	else
	{
		GetMesh()->UnHideBoneByName(TEXT("Neck_01"));
		bUseControllerRotationYaw = false;
	}
}

bool ASurvivalCharacter::IsSomethingInFocusRange(ABaseAICharacter*& Enemy, ABaseItem*& Item, float& OutDistance) const
{
	UCameraComponent* ActiveCamera;
	int LineTraceDistance;
	
	if (FirstPersonCamera->IsActive())
	{
		ActiveCamera = FirstPersonCamera;
		LineTraceDistance = FirstPerson;
	}
	else
	{
		ActiveCamera = ThirdPersonCamera;
		LineTraceDistance = ThirdPerson;
	}

	FHitResult HitResult;
	const FVector Start = ActiveCamera->GetComponentLocation();
	const FVector End = Start + (ActiveCamera->GetForwardVector() * LineTraceDistance);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	
	if (!bHit)
	{
		OutDistance = 0.f;
		return false;
	}
	
	Enemy = nullptr;
	Item = nullptr;

	AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor))
	{
		OutDistance = 0.f;
		return false;
	}

	OutDistance = FVector::Distance(Start, HitResult.ImpactPoint);
	
	if (HitActor->Implements<UFocusTarget>())
	{
		ETargetType Type = IFocusTarget::Execute_GetFocusType(HitActor);
		
		if (Type == ETargetType::Enemy)
		{
			Enemy = IFocusTarget::Execute_GetFocusedEnemy(HitActor);
			return true;
		}

		if (Type == ETargetType::Item)
		{
			Item = IFocusTarget::Execute_GetFocusedItem(HitActor);
			return true;
		}
	}
	
	return false;
}

void ASurvivalCharacter::UpdateFocus()
{
	ABaseAICharacter* NewFocusedEnemy = nullptr;
	ABaseItem* NewFocusedItem = nullptr;
	float Distance;
	IsSomethingInFocusRange(NewFocusedEnemy, NewFocusedItem, Distance);
	ASurvivalGameMode* GameMode = GetWorld()->GetAuthGameMode<ASurvivalGameMode>();
	if (IsValid(GameMode->GetHUDInstance()))
	{
		if (IsValid(NewFocusedEnemy))
		{
			const ABaseAICharacter* PreviousEnemy = FocusedEnemy;
			if (IsValid(FocusedEnemy) && FocusedEnemy != NewFocusedEnemy)
			{
				FocusedEnemy->OnHealthChanged.RemoveDynamic(GameMode->GetHUDInstance(), &USurvivalHUD::UpdateEnemyHealthBar);
			}
			FocusedEnemy = NewFocusedEnemy;
			if (FocusedEnemy != PreviousEnemy)
			{
				FocusedEnemy->OnHealthChanged.AddDynamic(GameMode->GetHUDInstance(), &USurvivalHUD::UpdateEnemyHealthBar);
				FocusedEnemy->OnHealthChanged.Broadcast(FocusedEnemy->GetCurrentHealth(), FocusedEnemy->GetMaxHealth(), FocusedEnemy->GetCharacterName());
			}
		}

		bool bIsItemInRange = false;
		
		if ((FirstPersonCamera->IsActive() && Distance <= 150.0f) ||
			(ThirdPersonCamera->IsActive() && Distance <= 325.0f) ||
			(CameraBoom->TargetArmLength == 250 && Distance <= 375.0f) ||
			(CameraBoom->TargetArmLength == 300 && Distance <= 425.0f) ||
			(CameraBoom->TargetArmLength == 350 && Distance <= 475.0f) ||
			(CameraBoom->TargetArmLength == 400 && Distance <= 525.0f))
		{
			bIsItemInRange = true;
		}

		OnFocusedItem.Broadcast(NewFocusedItem, bIsItemInRange);
	}
}

void ASurvivalCharacter::UpdateCharacterMovementRotation() const
{
	if (bIsWeaponReady)
	{
		if (ThirdPersonCamera->IsActive())
		{
			CameraBoom->TargetArmLength = 200.0f;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void ASurvivalCharacter::UpdateAimPitch()
{
	if (FirstPersonCamera->IsActive())
	{
		AimPitch = -FirstPersonCamera->GetComponentRotation().Pitch / 5;
	}
	else
	{
		constexpr float InterpSpeed = 8.0f;
		if (bIsWeaponReady)
		{
			AimPitch = FMath::FInterpTo(
				AimPitch,
				-ThirdPersonCamera->GetComponentRotation().Pitch / 5,
				GetWorld()->GetDeltaSeconds(),
				InterpSpeed
			);
		}
		else
		{
			AimPitch = FMath::FInterpTo(
				AimPitch,
				0.0f,
				GetWorld()->GetDeltaSeconds(),
				InterpSpeed
			);
		}
	}
}

float ASurvivalCharacter::GetAimPitch() const
{
	return AimPitch;
}

ABaseCharacter* ASurvivalCharacter::GetFocusedEnemy() const
{
	return FocusedEnemy;
}