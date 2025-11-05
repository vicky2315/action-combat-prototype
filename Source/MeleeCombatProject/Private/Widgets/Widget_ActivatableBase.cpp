// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_ActivatableBase.h"

AMenuPlayerController* UWidget_ActivatableBase::GetOwningFrontendController()
{
	if (OwningFrontEndPC.IsValid())
	{
		OwningFrontEndPC = GetOwningPlayer<AMenuPlayerController>();
	}

	return OwningFrontEndPC.IsValid() ? OwningFrontEndPC.Get(): nullptr;
}
