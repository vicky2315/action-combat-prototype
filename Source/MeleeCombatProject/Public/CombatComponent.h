// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/MainCharacter.h"
#include "CombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnAttackPerformedSignature,
	UCombatComponent,
	OnAttackPerformedDelegate,
	float, StaminaCost
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATPROJECT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackAnimMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HeavyAttackMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DodgeMontage;

	UAnimInstance* PlayerAnim;


	UPROPERTY(VisibleAnywhere)
	bool bCanAttack{ true };

	UPROPERTY(EditAnywhere)
	float StaminaCost{ 5.0f };


public:	
	// Sets default values for this component's properties
	UCombatComponent();

	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	UFUNCTION(BlueprintCallable)
	void BeginCharge();

	UFUNCTION(BlueprintCallable)
	void ReleaseCharge(float DamageMultiplier);

	UFUNCTION(BlueprintCallable)
	void HeavyAttack(float DamageMultiplier);

	UFUNCTION(BlueprintCallable)
	void BlockAttack();

	UFUNCTION(BlueprintCallable)
	void EnableCombo();

	UFUNCTION(BlueprintCallable)
	void DisableCombo();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UFUNCTION(BlueprintCallable)
	void SetDodgeState();

	UFUNCTION(BlueprintCallable)
	void DodgeAttack();

	UFUNCTION(BlueprintCallable)
	void SetBlockState();

	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DelayBeforeNextAttack{ 0.3f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DodgeDuration{ 0.3f };

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int ComboCounter{ 0 };

	AMainCharacter* CharacterRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();
		
};
