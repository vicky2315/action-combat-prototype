// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterStats.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATPROJECT_API UCharacterStats : public UDataAsset
{
	GENERATED_BODY()
	

public:

    // Attack stamina cost
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float StaminaCost = 10.f;

    //Initial stamina
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float Stamina = 10.f;

    //Max stamina
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float TotalStamina = 10.f;

    //Initial Health
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float Health = 10.f;

    //Max Health
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float MaxHealth = 10.f;

    //Damage of one strike
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float BaseDamage = 10.f;

    //Stamina regeneration rate
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float StaminaRegenRate = 10.f;

    //Delay before stamina regen
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float StaminaDelayDuration = 10.f;

    //Knockback force to be applied
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float KnockbackForce = 200.f;

    //Ignore
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float AttackSpeed = 1.0f;
};
