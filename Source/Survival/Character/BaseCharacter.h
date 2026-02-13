// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survival/Component/Inventory.h"
#include "Survival/Interface/Damageable.h"
#include "BaseCharacter.generated.h"

enum class ERace : uint8;
enum class ECharacterType : uint8;
class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, int, Current, int, Max, FText, Name);

UCLASS()
class SURVIVAL_API ABaseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Race", meta=(AllowPrivateAccess="true"))
	ERace Race;
	
	UPROPERTY(BlueprintReadOnly, Category="Character Type", meta=(AllowPrivateAccess="true"))
	ECharacterType CharacterType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Equipped", meta = (AllowPrivateAccess = "true"))
	AWeapon* WeaponEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Name", meta=(AllowPrivateAccess="true"))
	FText Name;

	UPROPERTY()
	FTimerHandle StaminaDrainTimer;

	UPROPERTY()
	FTimerHandle StaminaRegenTimer;

	UPROPERTY(BlueprintReadOnly, Category="Character State", meta=(AllowPrivateAccess="true"))
	bool bIsSprinting;

public:

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;
	
	ABaseCharacter();
	
	virtual void ModifyHealth_Implementation(const int Value) override;
	
	UFUNCTION(BlueprintGetter)
	ERace GetRace() const;
	
	UFUNCTION(BlueprintGetter)
	ECharacterType GetCharacterType() const;

	UFUNCTION(BlueprintGetter)
	AWeapon* GetWeaponEquipped() const;
	
	UFUNCTION(BlueprintGetter)
	FText GetCharacterName() const;

	UFUNCTION(BlueprintGetter)
	bool GetIsWeaponReady() const;
	
	UFUNCTION(BlueprintGetter)
	int GetCurrentHealth() const;

	UFUNCTION(BlueprintGetter)
	int GetMaxHealth() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UInventory* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health", meta=(AllowPrivateAccess="true"))
	int MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="Health", meta=(AllowPrivateAccess="true"))
	int CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stamina", meta=(AllowPrivateAccess="true"))
	int MaxStamina;

	UPROPERTY(BlueprintReadOnly, Category="Stamina", meta=(AllowPrivateAccess="true"))
	int CurrentStamina;
	
	UPROPERTY(BlueprintReadOnly, Category="Character State", meta=(AllowPrivateAccess="true"))
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly, Category="Character State", meta=(AllowPrivateAccess="true"))
	bool bCanMove;

	UPROPERTY(BlueprintReadWrite, Category="Character State", meta=(AllowPrivateAccess="true"))
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category="Character State", meta=(AllowPrivateAccess="true"))
	bool bIsWeaponReady;

	virtual void BeginPlay() override;

	void DrainStamina();

	virtual void RegenerateStamina();
	
	virtual void ToggleWeapon();

	void SetIsSprinting(const bool NewIsSprinting);
	
	void SetType(const ERace NewRace);

	UFUNCTION()
	void OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};
