// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerActions.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerInterface.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UPlayerActions::UPlayerActions()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerActions::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	MovementComp = CharacterRef->GetCharacterMovement();
	
	if (!CharacterRef->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass())) { return; }

	PlayerRef = Cast<IPlayerInterface>(CharacterRef);
	
}


// Called every frame
void UPlayerActions::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerActions::Sprint()
{
	if (!PlayerRef->HasEnoughStamina(SprintCost)) { 
		Walk();
		return; 
	}

	if (MovementComp->Velocity.Equals(FVector::ZeroVector)) { return; }

	MovementComp->MaxWalkSpeed = SprintSpeed;

	OnSprintDelegate.Broadcast(SprintCost);
}



void UPlayerActions::Walk()
{
	MovementComp->MaxWalkSpeed = OriginalSpeed;
}
