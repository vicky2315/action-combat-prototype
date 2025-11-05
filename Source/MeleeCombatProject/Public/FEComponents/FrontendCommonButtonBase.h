// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Subsystems/FrontendSubsystem.h"
#include "FrontendCommonButtonBase.generated.h"


class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class MELEECOMBATPROJECT_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public: 
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

private:
	UPROPERTY(meta = (BindWidgetOptional)) //Using this to bind other widgets to this widget
	UCommonTextBlock* CommonTextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true")) //Setting text before initializing
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUserUpperCaseForButtonTest = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;

	virtual void NativePreConstruct() override; //allows for previewing of widgets in editor before playing

	virtual void NativeOnCurrentTextStyleChanged() override;

	virtual void NativeOnHovered() override;

	virtual void NativeOnUnhovered() override;
};
