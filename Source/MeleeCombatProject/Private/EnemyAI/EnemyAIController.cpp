// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionSystem.h"
#include "Interfaces/PlayerInterface.h"
#include "Characters/EnemyCharacter.h"
#include "Enums/EAttackType.h"
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SetPerceptionComponent(*AIPerceptionComp);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Component"));

	if (SightConfig)
	{
		SightConfig->SightRadius = SightDistance;
		SightConfig->LoseSightRadius = LoseSightDistance;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionDegrees;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerceptionComp->ConfigureSense(*SightConfig);
		AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);

	BlackboardComp = GetBlackboardComponent();

}


void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CurrentEnemy = Cast<AEnemyCharacter>(InPawn);

}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{

		if (Actor->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
		{
			SetFocus(Actor);
			CurrentEnemy->PlayerDetected(Actor);
			//BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
		}
	}
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
		Super::OnMoveCompleted(RequestID, Result);
		
		if (IsValid(CurrentEnemy->TargetActor))
		{
			SetFocus(CurrentEnemy->TargetActor);
		}
}

EAttackType AEnemyAIController::ChooseAttack(float DistanceToPlayer, bool bCanLight, bool bCanHeavy)
{
	float LightScore = 0.f;
	float HeavyScore = 0.f;

	if (bCanLight)
	{
		LightScore = 1.0f; // base
		if (DistanceToPlayer < 250.f)
			LightScore += 1.0f;
	}

	if (bCanHeavy)
	{
		HeavyScore = 1.0f; // base
		if (DistanceToPlayer > 200.f && DistanceToPlayer < 500.f)
			HeavyScore += 2.0f;
	}

	return (HeavyScore > LightScore) ? EAttackType::Heavy : EAttackType::Light;
}
