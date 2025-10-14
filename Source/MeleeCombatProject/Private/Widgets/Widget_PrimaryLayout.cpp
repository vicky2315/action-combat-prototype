// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_PrimaryLayout.h"




UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetByTag(const FGameplayTag& StackTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(StackTag), TEXT("Cannot find the widget stack %s"), *StackTag.ToString());

	return RegisteredWidgetStackMap.FindRef(StackTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* WidgetToAdd)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, WidgetToAdd);

			//Debug::Print("WidgetStack Registered under Tag " + InStackTag.ToString());
		}
	}
}
