// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Controllers/MenuPlayerController.h"
#include "Widget_ActivatableBase.generated.h"


class AMenuPlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class MELEECOMBATPROJECT_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<AMenuPlayerController> OwningFrontEndPC;

protected:
	UFUNCTION(BlueprintPure)
	AMenuPlayerController* GetOwningFrontendController();

};
