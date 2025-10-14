// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EnemyAnimInstance.h"

void UEnemyAnimInstance::UpdateVelocity()
{
	APawn* EnemyRef = TryGetPawnOwner();

	if (!IsValid(EnemyRef)) { return; }

	FVector EnemyVelocity = EnemyRef->GetVelocity();

	CurrentVelocity = static_cast<float>(EnemyVelocity.Length());
}


void UEnemyAnimInstance::UpdateDirection()
{
	APawn* PawnRef{ TryGetPawnOwner() };

	if (!IsValid(PawnRef)) { return; }

	CurrentDirection = CalculateDirection(
		PawnRef->GetVelocity(), PawnRef->GetActorRotation()
	);
}