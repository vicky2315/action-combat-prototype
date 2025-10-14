// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DamageMultiplierValues.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATPROJECT_API UDamageMultiplierValues : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head")
	float HeadMultipler{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Torso")
	float TorsoMultipler{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leg")
	float LegMultipler{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arm")
	float ArmMultipler{ 0.0f };
};
