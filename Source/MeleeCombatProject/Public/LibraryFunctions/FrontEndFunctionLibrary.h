// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "FrontEndFunctionLibrary.generated.h"


class UWidget_ActivatableBase;
/**
 * 
 */
UCLASS()
class MELEECOMBATPROJECT_API UFrontEndFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassByTag( UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag);

};
