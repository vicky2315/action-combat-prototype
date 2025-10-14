// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentVelocity = 0.0f;

	UFUNCTION(BlueprintCallable)
	void UpdateVelocity();

	UFUNCTION(BlueprintCallable)
	void UpdateDirection();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentDirection{ 0.0f };
};
