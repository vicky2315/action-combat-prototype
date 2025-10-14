// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Animations/MainPlayerAnimInstance.h"
#include "Characters/StatsComponent.h"
#include "CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Combat/LockOnComponent.h"
#include "Characters/EnemyCharacter.h"
#include "Combat/TraceComponent.h"
#include "Characters/PlayerActions.h"
#include "Characters/EStat.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	LockComp = CreateDefaultSubobject<ULockOnComponent>(TEXT("Lock On Component"));

	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));

	PAComp = CreateDefaultSubobject<UPlayerActions>(TEXT("Player Actions Component"));

	SwordHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordHitbox"));
	SwordHitbox->InitCapsuleSize(5.f, 50.f);
	SwordHitbox->SetupAttachment(GetMesh(), TEXT("ik_hand_gun"));
	SwordHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	SwordHitbox->SetCollisionObjectType(ECC_WorldDynamic);
	SwordHitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	SwordHitbox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnim = Cast<UMainPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//SwordHitbox->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnSwordOverlap);

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMainCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::BaseDamage];
}

bool AMainCharacter::HasEnoughStamina(float Cost)
{
	return StatsComp->Stats[EStat::Stamina] >= Cost;
}

ECombatState AMainCharacter::GetCombatState()
{
	return CurrentState;
}

void AMainCharacter::EnableSwordCollision()
{
	SwordHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainCharacter::DisableSwordCollision()
{
	SwordHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::OnSwordOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor))
	//{
		UE_LOG(LogTemp, Log, TEXT("Sword hit enemy part: %s"), *OtherComp->GetName());
	//	Enemy->TakeDamage(BaseDamage, FDamageEvent(), GetController(), this);
	//}

		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor))
		{
			TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

			UGameplayStatics::ApplyDamage(
				OtherActor,
				BaseDamage,       
				GetInstigatorController(),
				this,
				DamageTypeClass
			);
		}
}

void AMainCharacter::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	StatsComp->ReduceHealth(Damage);
	UE_LOG(LogTemp, Warning, TEXT("MainCharacter took %f damage, remaining health"), Damage);
}
