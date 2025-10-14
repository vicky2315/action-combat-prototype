// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockOnComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/BPI_Enemy.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnerRef->GetCharacterMovement();
	springArmComp = OwnerRef->GetComponentByClass<USpringArmComponent>();

	// ...
	
}

void ULockOnComponent::StartLockOn(float Radius)
{
	FHitResult outResult;
	FVector startLocation { OwnerRef->GetActorLocation() };
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(Radius) };
	FCollisionQueryParams IgnoreActors{	
		FName{ TEXT("Ignore Collision Params") },
		false,
		OwnerRef
	};

	bool bHasFoundTarget = GetWorld()->SweepSingleByChannel(
		outResult, 
		startLocation, 
		startLocation, 
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		IgnoreActors
	);

	if (!bHasFoundTarget) { return; }

	if (!outResult.GetActor()->Implements<UBPI_Enemy>()) { return; }

	Controller->SetIgnoreLookInput(true);
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = true;  //rotation is smooth when its changed
	TargetActor = outResult.GetActor();

	IBPI_Enemy::Execute_onSelect(TargetActor);

	OnUpdatedTargetDelegate.Broadcast(TargetActor);
}


// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsValid(TargetActor)) { return; }
		
	EnemyLocation = TargetActor->GetActorLocation();
	PlayerLocation = OwnerRef->GetActorLocation();

	DistanceFromEnemy = FVector::Distance(PlayerLocation, EnemyLocation);

	if (DistanceFromEnemy >= BreakDistance)
	{
		EndLockOn();
		return;
	}

	EnemyLocation.Z -= 125.0f;

	lookAtEnemyRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, EnemyLocation);

	Controller->SetControlRotation(lookAtEnemyRotation);

	springArmComp->TargetOffset = FVector(0, 0, 150.0);

}

void ULockOnComponent::EndLockOn() 
{
	IBPI_Enemy::Execute_deSelect(TargetActor);
	
	TargetActor = nullptr;

	springArmComp->TargetOffset = FVector(0, 0, 0);

	Controller->SetIgnoreLookInput(false);
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;

	Controller->ResetIgnoreLookInput();

	OnUpdatedTargetDelegate.Broadcast(TargetActor);
}

void ULockOnComponent::ToggleLockOn(float Radius)
{
	if (IsValid(TargetActor))
	{
		EndLockOn();
	}
	else
	{
		StartLockOn(Radius);
	}
}
