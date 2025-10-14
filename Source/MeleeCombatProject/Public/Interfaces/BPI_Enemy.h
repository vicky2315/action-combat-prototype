// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BPI_Enemy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBPI_Enemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MELEECOMBATPROJECT_API IBPI_Enemy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent)
	void onSelect();

	UFUNCTION(BlueprintImplementableEvent)
	void deSelect();

	UFUNCTION(BlueprintImplementableEvent)
	void DamageReceived();

	
};
