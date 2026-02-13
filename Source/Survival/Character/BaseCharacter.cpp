#include "Survival/Character/BaseCharacter.h"
#include "Survival/Component/Inventory.h"
#include "RaceList.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Survival/Item/Weapon/Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	WeaponComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponComponent"));
	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	SetType(ERace::Human);
	MaxHealth = 1;
	MaxStamina = 1;
	bIsSprinting = false;
	bIsAttacking = false;
	bCanMove = true;
	bIsFalling = false;
	bIsWeaponReady = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	if (IsValid(WeaponComponent->GetChildActor()))
	{
		if (WeaponComponent->GetChildActor()->Implements<UFocusTarget>())
		{
			ETargetType Type = IFocusTarget::Execute_GetFocusType(WeaponComponent->GetChildActor());
			if (Type == ETargetType::Weapon)
			{
				WeaponEquipped = IFocusTarget::Execute_GetFocusedWeapon(WeaponComponent->GetChildActor());
				WeaponEquipped->SetWeaponOwner(this);
			}
		}
	}
}

void ABaseCharacter::ModifyHealth_Implementation(const int Value)
{
	CurrentHealth += Value;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, Name);
	if (CurrentHealth <= 0)
	{
		OnHealthChanged.RemoveAll(this);
		Destroy();
	}
}

void ABaseCharacter::DrainStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina - 1.f, 0.f, MaxStamina);

	if (CurrentStamina <= 0.f)
	{
		SetIsSprinting(false);
	}
}

void ABaseCharacter::RegenerateStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina + 1.f, 0.f, MaxStamina);
}

void ABaseCharacter::ToggleWeapon()
{
	if (!bIsWeaponReady)
	{
		bIsWeaponReady = true;
	}
	else
	{
		bIsWeaponReady = false;
	}
}

void ABaseCharacter::SetIsSprinting(const bool NewIsSprinting)
{
	if (NewIsSprinting && bIsAttacking)
	{
		return;
	}
	
	if (bIsSprinting == NewIsSprinting)
	{
		return;
	}

	if (NewIsSprinting && CurrentStamina <= 0.f)
	{
		return;
	}
	
	bIsSprinting = NewIsSprinting;
	
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		GetWorldTimerManager().ClearTimer(StaminaRegenTimer);
		GetWorldTimerManager().SetTimer(StaminaDrainTimer, this, &ABaseCharacter::DrainStamina, 0.2f, true);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		GetWorldTimerManager().ClearTimer(StaminaDrainTimer);
		GetWorldTimerManager().SetTimer(StaminaRegenTimer,this, &ABaseCharacter::RegenerateStamina,0.2f,true,5.0f );
	}
}

void ABaseCharacter::SetType(const ERace NewRace)
{
	FRaceList List;
	this->Race = NewRace;
	if (const ECharacterType* CharType = List.Race.Find(this->Race))
	{
		CharacterType = *CharType;
	}
}

void ABaseCharacter::OnMeleeAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	bCanMove = true;
}

ERace ABaseCharacter::GetRace() const
{
	return Race;
}

ECharacterType ABaseCharacter::GetCharacterType() const
{
	return CharacterType;
}

AWeapon* ABaseCharacter::GetWeaponEquipped() const
{
	return WeaponEquipped;
}

FText ABaseCharacter::GetCharacterName() const
{
	return Name;
}

bool ABaseCharacter::GetIsWeaponReady() const
{
	return bIsWeaponReady;
}

int ABaseCharacter::GetCurrentHealth() const
{
	return CurrentHealth;
}

int ABaseCharacter::GetMaxHealth() const
{
	return MaxHealth;
}