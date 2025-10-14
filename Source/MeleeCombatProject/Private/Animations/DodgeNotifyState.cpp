// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/DodgeNotifyState.h"
#include "CombatComponent.h"

void UDodgeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	CombatComp = MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>();

	if (!IsValid(CombatComp)) { return; }

	CombatComp->CharacterRef->CurrentState = ECombatState::Dodge;
}


void UDodgeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	CombatComp = MeshComp->GetOwner()->FindComponentByClass<UCombatComponent>();

	if (!IsValid(CombatComp)) { return; }

	CombatComp->HandleResetAttack();
}