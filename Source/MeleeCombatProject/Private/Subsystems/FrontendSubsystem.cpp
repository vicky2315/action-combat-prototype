// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/FrontendSubsystem.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widgets/Widget_PrimaryLayout.h"


UFrontendSubsystem* UFrontendSubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UFrontendSubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontendSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UFrontendSubsystem::RegisterPrimaryLayout(UWidget_PrimaryLayout* InCreatedLayout)
{
	check(InCreatedLayout);
	CreatedPrimaryLayout = InCreatedLayout;

	Debug::Print("Primary Layout has been stored");
}

void UFrontendSubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& inWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallback)
{
	check(!SoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(SoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(															//Called after the soft widget class is loaded
			[SoftWidgetClass, this, inWidgetStackTag, AsyncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = SoftWidgetClass.Get();
				check(LoadedWidgetClass && CreatedPrimaryLayout)
																								   //Now getting the stack of widgets of the specified tag
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetByTag(inWidgetStackTag);

				UWidget_ActivatableBase* CreatedWidget =  FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(LoadedWidgetClass,            //Using the (Widget, TFunction) overload because lambda inside T function is called after the instance is generated and before the widget is pushed, lets us do any changes required to the widget before push).
					[AsyncPushStateCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
					{
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);					//AsyncPushStateCallback has not been initialized it, just a TFunction for now, that helps in doing things before or after a widget is initialized.
					}
				);		

				AsyncPushStateCallback(EAsyncPushWidgetState::CreatedAfterPush, CreatedWidget);										//after
			}
		)

	);
}
