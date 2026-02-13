#include "Weapon.h"
#include "WeaponList.h"
#include "Components/BoxComponent.h"
#include "Survival/Interface/Damageable.h"

AWeapon::AWeapon()
{
	SetType(EWeaponType::Unarmed);
	Damage = 1;
	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	RootComponent = HitBox;
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBox->SetCollisionObjectType(ECC_WorldDynamic);
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnHit);
	
	StaticMesh->SetSimulatePhysics(false);

	ItemData.IsStackable = false;
}

void AWeapon::SetType(const EWeaponType Type)
{
	FWeaponList List;
	WeaponType = Type;
	if (const ECombatStyle* Style = List.Weapon.Find(WeaponType))
	{
		CombatStyle = *Style;
	}
}

ETargetType AWeapon::GetFocusType_Implementation() const
{
	return ETargetType::Weapon;
}

AWeapon* AWeapon::GetFocusedWeapon_Implementation()
{
	return this;
}

void AWeapon::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == WeaponOwner)
	{
		return;
	}

	if (HitActorsThisAttack.Contains(OtherActor))
	{
		return;
	}

	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		HitActorsThisAttack.Add(OtherActor);
		IDamageable::Execute_ModifyHealth(OtherActor, -Damage);
	}
}

EWeaponType AWeapon::GetWeaponType() const
{
	return WeaponType;
}

ECombatStyle AWeapon::GetCombatStyle() const
{
	return CombatStyle;
}

int AWeapon::GetDamage() const
{
	return Damage;
}

UBoxComponent* AWeapon::GetHitBox() const
{
	return HitBox;
}

TSet<TWeakObjectPtr<AActor>>& AWeapon::GetHitActorsThisAttack()
{
	return HitActorsThisAttack;
}

void AWeapon::SetWeaponOwner(AActor* NewOwner)
{
	WeaponOwner = NewOwner;
}