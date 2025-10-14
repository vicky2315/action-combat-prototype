// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ECombatState 
{
	Idle UMETA(DisplayName = "Idle"),
	Attacking UMETA(DisplayName = "Attacking"),
	Combos UMETA(DisplayName = "ComboAttack"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
	Recovery UMETA(DisplayName = "Recovery"),
	Dodge UMETA(DisplayName = "Dodging"),
	Block UMETA(DisplayName = "Block")
};