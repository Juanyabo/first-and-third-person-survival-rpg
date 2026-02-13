#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Survival/Item/BaseItem.h"
#include "Weapon.generated.h"

enum class EWeaponType : uint8;
enum class ECombatStyle : uint8;
class UBoxComponent;

UCLASS()
class SURVIVAL_API AWeapon : public ABaseItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Type", meta=(AllowPrivateAccess="true"))
	EWeaponType WeaponType;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat Style", meta=(AllowPrivateAccess="true"))
	ECombatStyle CombatStyle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage", meta=(AllowPrivateAccess="true"))
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision", meta=(AllowPrivateAccess="true"))
	UBoxComponent* HitBox;

	UPROPERTY()
	AActor* WeaponOwner;

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> HitActorsThisAttack;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:	
	AWeapon();

	UFUNCTION(BlueprintGetter)
	EWeaponType GetWeaponType() const;

	UFUNCTION(BlueprintGetter)
	ECombatStyle GetCombatStyle() const;

	UFUNCTION(BlueprintGetter)
	int GetDamage() const;

	UFUNCTION(BlueprintGetter)
	UBoxComponent* GetHitBox() const;
	
	UFUNCTION()
	TSet<TWeakObjectPtr<AActor>>& GetHitActorsThisAttack();
	
	UFUNCTION(BlueprintSetter)
	void SetWeaponOwner(AActor* NewOwner);

protected:

	void SetType(const EWeaponType Type);

	virtual ETargetType GetFocusType_Implementation() const override;
	
	virtual AWeapon* GetFocusedWeapon_Implementation() override;

};
