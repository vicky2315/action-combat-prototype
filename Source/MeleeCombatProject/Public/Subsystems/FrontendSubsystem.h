// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Debug/FrontendDebugHelper.h"
#include "FrontendSubsystem.generated.h"

/**
 * 
 */
class UWidget_PrimaryLayout;
class UWidget_ActivatableBase;
struct FGameplayTag;
class UFrontendCommonButtonBase;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	CreatedAfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, UFrontendCommonButtonBase*, BroadcastingButton, FText, DescriptionText);

UCLASS()
class MELEECOMBATPROJECT_API UFrontendSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UFrontendSubsystem* Get(const UObject* WorldContextObject);
	
	//To make sure there is only one instance of this subsystem running
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;

	UFUNCTION(BlueprintCallable)
	void RegisterPrimaryLayout(UWidget_PrimaryLayout* InCreatedLayout);

	void PushSoftWidgetToStackAsync(const FGameplayTag& inWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback);

private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
};
