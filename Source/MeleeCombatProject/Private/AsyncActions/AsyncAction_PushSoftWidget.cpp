// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncActions/AsyncAction_PushSoftWidget.h"
#include "Subsystems/FrontendSubsystem.h"
#include "Widgets/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject, APlayerController* OwningPlayerController, TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, const UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("Soft Widget class was passed a null value"));

	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushSoftWidget* Node = NewObject<UAsyncAction_PushSoftWidget>();

			Node->CachedOwningWorld = World;
			Node->CachedPlayerController = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->CachedbFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);										//used to register an async action with the game's Game Instance. It ensures that the async node is tied to the game's lifecycle and can be properly managed, especially when the engine needs to handle long-running operations or data that must persist across different levels.
		
			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	UFrontendSubsystem* FrontEndSubSystem = UFrontendSubsystem::Get(CachedOwningWorld.Get());

	FrontEndSubSystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedSoftWidgetClass, [this](EAsyncPushWidgetState PushedWidgetState, UWidget_ActivatableBase* PushedWidget)
		{
			switch (PushedWidgetState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				PushedWidget->SetOwningPlayer(CachedPlayerController.Get());
				BeforeWidgetPush.Broadcast(PushedWidget); 
				break;
			case EAsyncPushWidgetState::CreatedAfterPush:
				AfterWidgetPush.Broadcast(PushedWidget);
				if (CachedbFocusOnNewlyPushedWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusWidget())
					{
						WidgetToFocus->SetFocus();
					}
				}

				SetReadyToDestroy();												//unregisters action/function from game instance to reduce load

				break;
			default : 
				break;
			}
		}
	);
}
