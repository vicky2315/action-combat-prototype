// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Gameframework/Character.h"
#include "Characters/MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/PlayerInterface.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CharacterRef = GetOwner<AMainCharacter>();
	PlayerAnim = CharacterRef->GetMesh()->GetAnimInstance();
	
}

void UCombatComponent::ComboAttack()
{
	if (CharacterRef->Implements<UPlayerInterface>())
	{
		IPlayerInterface* OwnerRef = Cast<IPlayerInterface>(CharacterRef);

		if (OwnerRef && !OwnerRef->HasEnoughStamina(StaminaCost))
		{
			return;
		}

	}

	//if (!bCanAttack) { return; }

	if (CharacterRef->CurrentState != ECombatState::Idle && CharacterRef->CurrentState != ECombatState::Combos) { return; }

	//bCanAttack = false;
	CharacterRef->CurrentState = ECombatState::Attacking;

	CharacterRef->PlayAnimMontage(AttackAnimMontage[ComboCounter],1.0f, NAME_None);

	ComboCounter++;

	int MaxCombo = AttackAnimMontage.Num() - 1;

	ComboCounter = UKismetMathLibrary::Wrap(
		ComboCounter, -1, MaxCombo
	);

	OnAttackPerformedDelegate.Broadcast(StaminaCost);
}

void UCombatComponent::HeavyAttack(float DamageMultiplier)
{
	if (CharacterRef->CurrentState != ECombatState::Idle && CharacterRef->CurrentState != ECombatState::Combos) { return; }

	CharacterRef->CurrentState = ECombatState::Attacking;

	CharacterRef->PlayAnimMontage(HeavyAttackMontage);

	ComboCounter = 0;

}


void UCombatComponent::BeginCharge()
{
	if (PlayerAnim)
	{
		if (CharacterRef->CurrentState != ECombatState::Idle && CharacterRef->CurrentState != ECombatState::Combos) { return; }

		PlayerAnim->Montage_Play(HeavyAttackMontage, 0.5f);
	}
}

void UCombatComponent::ReleaseCharge(float DamageMultiplier)
{
	if (PlayerAnim)
	{
		//PlayerAnim->Montage_Pause(HeavyAttackMontage);
		PlayerAnim->Montage_SetPlayRate(HeavyAttackMontage,1.0f);	
		//PlayerAnim->Montage_Resume(HeavyAttackMontage);
	}
}

void UCombatComponent::DodgeAttack()
{
	if (CharacterRef->CurrentState==Idle && CharacterRef->CurrentState != ECombatState::Attacking)
	{
		float PlayRate = DodgeMontage->GetPlayLength()/DodgeDuration;
		//PlayerAnim->Montage_Play(DodgeMontage, PlayRate);
		CharacterRef->PlayAnimMontage(DodgeMontage, PlayRate);
	}
}


void UCombatComponent::BlockAttack()
{
	if (CharacterRef->CurrentState == Idle && CharacterRef->CurrentState != ECombatState::Attacking)
	{
		CharacterRef->StopAnimMontage();
	}
	CharacterRef->PlayAnimMontage(BlockMontage);
}

void UCombatComponent::SetDodgeState()
{
	CharacterRef->CurrentState = ECombatState::Dodge;
}

void UCombatComponent::SetBlockState()
{
	CharacterRef->CurrentState = ECombatState::Block;
}

void UCombatComponent::EnableCombo()
{
	CharacterRef->CurrentState = ECombatState::Combos;
}


void UCombatComponent::DisableCombo()
{
	CharacterRef->CurrentState = ECombatState::Attacking;
}

void UCombatComponent::EndAttack()
{
	CharacterRef->CurrentState = ECombatState::Recovery;
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::HandleResetAttack()
{
	//bCanAttack = true;
	CharacterRef->CurrentState = ECombatState::Idle;
	ComboCounter = 0;
}


