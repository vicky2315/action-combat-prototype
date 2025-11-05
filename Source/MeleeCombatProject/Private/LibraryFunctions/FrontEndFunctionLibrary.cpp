// Fill out your copyright notice in the Description page of Project Settings.


#include "LibraryFunctions/FrontEndFunctionLibrary.h"
#include "DeveloperSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontEndFunctionLibrary::GetFrontendSoftWidgetClassByTag( UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag)
{
	const UFrontendDeveloperSettings* FDeveloperSettings =  GetDefault<UFrontendDeveloperSettings>();
	
	checkf(FDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag), TEXT("Could not find widget tag %s"), *InWidgetTag.ToString());

	return FDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}
