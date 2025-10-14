// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/FighterInterface.h"
#include "Interfaces/FighterInterface.h"
#include "Camera/CameraShakeBase.h"
#include "Engine/DamageEvents.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	MeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) { return; }
	
	StartSocketLocation =  MeshComp->GetSocketLocation(StartSocket);

	EndSocketLocation = MeshComp->GetSocketLocation(EndSocket);

	SocketRotation = MeshComp->GetSocketQuaternion(SRotation);

	WeaponDistance = FVector::Distance(StartSocketLocation, EndSocketLocation);

	BoxSize = { BoxCollisionLength, BoxCollisionLength, WeaponDistance };
	BoxSize /= 2;

	FCollisionShape Box(
		FCollisionShape::MakeBox(BoxSize)
	);

	FCollisionQueryParams IgnoreActors{
		FName { TEXT("Ignore Params")},
		false,
		GetOwner()
	};

	bool hitTarget = GetWorld()->SweepMultiByChannel(
		OutResults,
		StartSocketLocation,
		EndSocketLocation,
		SocketRotation,
		ECollisionChannel::ECC_GameTraceChannel1,
		Box,
		IgnoreActors
	);

	if (bDebugMode)
	{
		FVector CenterPoint = UKismetMathLibrary::VLerp(
			StartSocketLocation, EndSocketLocation, 0.5f
		);

		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(),
			CenterPoint,
			Box.GetExtent(),
			hitTarget ? FLinearColor::Red : FLinearColor::Green,
			SocketRotation.Rotator(),
			1.0f,
			2.0f
		);
	}

	if (OutResults.Num() == 0) { return; }

	float CharacterDamage = 0.0f;

	IFighterInterface* CharacterRef = Cast<IFighterInterface>(GetOwner());

	if (CharacterRef)
	{
		CharacterRef->EnableSwordCollision();
		CharacterDamage = CharacterRef->GetDamage();
	}

	AActor* TargetActor{};

	FDamageEvent TargetAttackedEvent;

	for (const FHitResult& Hit : OutResults)
	{
		TargetActor = Hit.GetActor();

		if (ActorsDamaged.Contains(TargetActor)) { continue; }
		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);

		PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			PC->ClientStartCameraShake(HitCameraShake);
		}

		ActorsDamaged.AddUnique(TargetActor);

		if (HitNiagaraSystem)
		{
			SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				HitNiagaraSystem,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation()
			);
		}
	}
}

void UTraceComponent::HandleResetAttack()
{
	ActorsDamaged.Empty();

	IFighterInterface* CharacterRef = Cast<IFighterInterface>(GetOwner());

	if (CharacterRef)
	{
		CharacterRef->DisableSwordCollision();
	}
	
}

