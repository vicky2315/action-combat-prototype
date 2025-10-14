// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/BTTasks/BTT_LightAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"


EBTNodeResult::Type UBTT_LightAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>() };

	if (!IsValid(CharacterRef))
	{
		return EBTNodeResult::Failed;
	}

	CharacterRef->PlayAnimMontage(LightAttackMontage);
	return EBTNodeResult::Succeeded;
}