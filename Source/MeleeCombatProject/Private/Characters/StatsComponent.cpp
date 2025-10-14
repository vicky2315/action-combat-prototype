// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterStatsData)
	{
		Stats[EStat::Stamina] = CharacterStatsData->Stamina;
		Stats[EStat::MaxStamina] = CharacterStatsData->TotalStamina;
		Stats[EStat::Health] = CharacterStatsData->Health;
		Stats[EStat::MaxHealth] = CharacterStatsData->MaxHealth;
		Stats[EStat::BaseDamage] = CharacterStatsData->BaseDamage;
	}
	
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::ReduceHealth(float Damage)
{
	if (Stats[EStat::Health] <= 0) { return; }

	Stats[EStat::Health] -= Damage; 

	Stats[EStat::Health] = UKismetMathLibrary::FClamp(
		Stats[EStat::Health],
		0.0,
		Stats[EStat::MaxHealth]
	);
}

void UStatsComponent::ReduceStamina(float Amount)
{
	Stats[EStat::Stamina] -= Amount;

	Stats[EStat::Stamina] = UKismetMathLibrary::FClamp(
		Stats[EStat::Stamina],
		0,
		Stats[EStat::MaxStamina]
	);

	bCanRegen = false;

	FLatentActionInfo RegenPlaceholder{
		0,
		100,
		TEXT("StartRegen"),
		this
	};

	UKismetSystemLibrary::RetriggerableDelay(
		GetWorld(),
		StaminaDelayDuration,
		RegenPlaceholder
		);
}

void UStatsComponent::RegenStamina()
{
	if (!bCanRegen) { return; }

	Stats[EStat::Stamina] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[EStat::Stamina],
		Stats[EStat::MaxStamina],
		GetWorld()->DeltaTimeSeconds,
		StaminaRegenRate
	);
}

void UStatsComponent::StartRegen()
{
	bCanRegen = true;
}