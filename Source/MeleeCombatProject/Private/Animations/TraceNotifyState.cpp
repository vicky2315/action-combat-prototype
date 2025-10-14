// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/TraceNotifyState.h"
#include "Combat/TraceComponent.h"

void UTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* TraceRef = MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>();

	if (!IsValid(TraceRef)) { return; }

	TraceRef->bIsAttacking = true;
}

void UTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* TraceRef = MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>();

	if (!IsValid(TraceRef)) { return; }

	TraceRef->bIsAttacking = false;
}