// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/MainPlayerAnimInstance.h"

void UMainPlayerAnimInstance::UpdateVelocity()
{
	APawn* PawnRef{ TryGetPawnOwner() };

	if (!IsValid(PawnRef)) { return; }

	FVector PVelocity{ PawnRef->GetVelocity() };

	CurrentVelocity = static_cast<float>(PVelocity.Length());

}

void UMainPlayerAnimInstance::HandleUpdatedTarget(AActor* UpdatedActor)
{
	bIsInCombat = IsValid(UpdatedActor);
}

void UMainPlayerAnimInstance::UpdateDirection()
{
	APawn* PawnRef{ TryGetPawnOwner() };

	if (!IsValid(PawnRef)) { return; }

	if (!bIsInCombat) {
		return;
	}

	CurrentDirection = CalculateDirection(
		PawnRef->GetVelocity(), PawnRef->GetActorRotation()
	);
}