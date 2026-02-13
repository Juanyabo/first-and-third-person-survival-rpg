#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Survival/Character/BaseCharacter.h"
#include "Survival/Item/BaseItem.h"
#include "SurvivalCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ABaseAICharacter;
class ABaseItem;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, int, Current, int, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFocusedItem, ABaseItem*, Item, bool, IsInRange);

UCLASS()
class SURVIVAL_API ASurvivalCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY()
	ABaseAICharacter* FocusedEnemy;
	
	UPROPERTY(BlueprintReadWrite, Category="Camera", meta=(AllowPrivateAccess="true"))
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Distance to tag enemy", meta=(AllowPrivateAccess="true"))
	int FirstPerson;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Distance to tag enemy", meta=(AllowPrivateAccess="true"))
	int ThirdPerson;

	UFUNCTION()
	bool IsSomethingInFocusRange(ABaseAICharacter*& Enemy, ABaseItem*& Item, float& OutDistance) const;
	
	UFUNCTION()
	void UpdateFocus();

	UFUNCTION()
	void UpdateCharacterMovementRotation() const;
	
	UFUNCTION()
	void UpdateAimPitch();
	
public:

	UPROPERTY(BlueprintAssignable, Category="Stamina")
	FOnStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnFocusedItem OnFocusedItem;

	ASurvivalCharacter();
	
	UFUNCTION(BlueprintGetter)
	float GetAimPitch() const;

	UFUNCTION()
	ABaseCharacter* GetFocusedEnemy() const;

protected:
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SwitchCameraAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SheathWeaponAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InventoryAction;

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void RegenerateStamina() override;
	
	virtual void ToggleWeapon() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);

	void UpdateCamera(const FInputActionValue& Value);

	void SheathWeapon(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void ShowInventory(const FInputActionValue& Value);

	void ActivateCamera(UCameraComponent* Camera);
	
};
