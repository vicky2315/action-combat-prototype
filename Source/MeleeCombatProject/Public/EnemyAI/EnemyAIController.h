// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/EnemyCharacter.h"
#include "Enums/EAttackType.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.generated.h"


class AEnemyCharacter;
/**
 * 
 */
UCLASS()
class MELEECOMBATPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SightDistance{ 2000.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LoseSightDistance{ 3000.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PeripheralVisionDegrees{ 90.0f };

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	AEnemyCharacter* CurrentEnemy;

	class UBlackboardComponent* BlackboardComp;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	EAttackType ChooseAttack(float DistanceToPlayer, bool bCanLight, bool bCanHeavy);
};
