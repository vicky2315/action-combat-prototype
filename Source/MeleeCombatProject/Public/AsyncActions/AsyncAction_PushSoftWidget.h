// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushSoftWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoftObjectPushDelegate, UWidget_ActivatableBase*, PushedWidget);


class UWidget_ActivatableBase;
/**
 * 
 */
UCLASS()
class MELEECOMBATPROJECT_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = true, DisplayName = "Push Soft Widget to Widget Stack"))
	static UAsyncAction_PushSoftWidget* PushSoftWidget(const UObject* WorldContextObject, 
		APlayerController* OwningPlayerController,
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, 
		UPARAM(meta = (Categories = "Frontend.Widget")) const FGameplayTag InWidgetStackTag,
		bool bFocusOnNewlyPushedWidget = true);


	UPROPERTY(BlueprintAssignable)
	FOnSoftObjectPushDelegate BeforeWidgetPush;

	UPROPERTY(BlueprintAssignable)
	FOnSoftObjectPushDelegate AfterWidgetPush;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedPlayerController;
	TSoftClassPtr<UWidget_ActivatableBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool CachedbFocusOnNewlyPushedWidget = false;

	virtual void Activate() override;
	
};
