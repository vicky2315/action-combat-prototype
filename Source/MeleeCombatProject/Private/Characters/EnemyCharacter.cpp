// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "EnemyAI/EnemyAIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/FighterInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/ECombatState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Combat/TraceComponent.h"
#include "Characters/StatsComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));

	LeftTraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Left Blade Trace Component"));

	RightTraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Right Blade Trace Component"));


		HeadHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadHitbox"));
		HeadHitbox->SetupAttachment(GetMesh(), FName("pelvis"));
		HeadHitbox->SetCapsuleHalfHeight(20.f);
		HeadHitbox->SetCapsuleRadius(15.f);
		HeadHitbox->SetCollisionProfileName("Hitbox");

		TorsoHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TorsoHitbox"));
		TorsoHitbox->SetupAttachment(GetMesh(), FName("spine_03"));
		TorsoHitbox->SetCapsuleHalfHeight(40.f);
		TorsoHitbox->SetCapsuleRadius(25.f);
		TorsoHitbox->SetCollisionProfileName("Hitbox");


		LeftArmHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArmHitBox"));
		LeftArmHitbox->SetupAttachment(GetMesh(), FName("ik_hand_l"));
		LeftArmHitbox->SetCapsuleHalfHeight(40.f);
		LeftArmHitbox->SetCapsuleRadius(25.f);
		LeftArmHitbox->SetCollisionProfileName("Hitbox");

		RightArmHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightArmHitBox"));
		RightArmHitbox->SetupAttachment(GetMesh(), FName("ik_hand_r"));
		RightArmHitbox->SetCapsuleHalfHeight(40.f);
		RightArmHitbox->SetCapsuleRadius(25.f);
		RightArmHitbox->SetCollisionProfileName("Hitbox");

		LeftLegHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftLegHitBox"));
		LeftLegHitbox ->SetupAttachment(GetMesh(), FName("foot_l"));
		LeftLegHitbox->SetCapsuleHalfHeight(40.f);
		LeftLegHitbox ->SetCapsuleRadius(25.f);
		LeftLegHitbox->SetCollisionProfileName("Hitbox");

		RightLegHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightLegHitBox"));
		RightLegHitbox->SetupAttachment(GetMesh(), FName("foot_r"));
		RightLegHitbox->SetCapsuleHalfHeight(40.f);
		RightLegHitbox->SetCapsuleRadius(25.f);
		RightLegHitbox->SetCollisionProfileName("Hitbox");


		HeadHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHitboxOverlap);
		TorsoHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHitboxOverlap);
		LeftArmHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHitboxOverlap);
		RightArmHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHitboxOverlap);
		LeftLegHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHitboxOverlap);
		RightLegHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnHitboxOverlap);
}

	// Called when the game starts or when spawned
	void AEnemyCharacter::BeginPlay()
	{
		Super::BeginPlay();

		BlackboardComp = GetController<AEnemyAIController>()->GetBlackboardComponent();

		BlackboardComp->SetValueAsEnum(
			TEXT("CurrentState"),
			InitialState
		);
	}

	// Called every frame
	void AEnemyCharacter::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);

		if (!bTrainingDummyMode) { return; }

		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		if (MoveComp)
		{
			MoveComp->StopMovementImmediately();
		}

		AEnemyAIController* EnemyAICon = Cast<AEnemyAIController>(GetController());
		if (EnemyAICon && EnemyAICon->BrainComponent)
		{
			EnemyAICon->BrainComponent->StopLogic("Training Dummy Mode Active");
		}

	}

	void AEnemyCharacter::Death()
	{
		InitialState = EEnemyState::Death;
		AEnemyAIController* EnemyAICon = Cast<AEnemyAIController>(GetController());
		if (EnemyAICon && EnemyAICon->BrainComponent)
		{
			EnemyAICon->BrainComponent->StopLogic("Enemy Dead");
		}
	}

	// Called to bind functionality to input
	void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);

	}

	void AEnemyCharacter::PlayerDetected(AActor* DetectedActor)
	{
		BlackboardComp->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Range);
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), DetectedActor);
		TargetActor = DetectedActor;
	}


	void AEnemyCharacter::LightAttack()
	{
		PlayAnimMontage(LightAttackMontage);
	}

	void AEnemyCharacter::HeavyAttack()
	{
		PlayAnimMontage(HeavyAttackMontage);
	}

	void AEnemyCharacter::HandleDamage(float Damage, FVector HitLocation, FVector InstigatorLocation)
	{
		StatsComp->ReduceHealth(Damage);

		PlayHitReaction();
		PlayHitEffect(HitLocation);

		FVector KnockbackDir = GetActorLocation() - InstigatorLocation;
		ApplyKnockback(KnockbackDir, 600.f);
	}

	void AEnemyCharacter::ApplyKnockback(const FVector& HitDirection, float Strength)
	{
		if (bTrainingDummyMode) { return; }

		UCharacterMovementComponent* MoveComp = GetCharacterMovement();

		if (MoveComp)
		{
			MoveComp->StopMovementImmediately();

			FVector KnockbackDir = HitDirection.GetSafeNormal();

			LaunchCharacter(KnockbackDir * Strength, true, true);
		}
	}
	
	void AEnemyCharacter::PlayHitReaction()
	{
		PlayAnimMontage(HitReactionMontage);
	}


	void AEnemyCharacter::PlayHitEffect(const FVector& HitLocation)
	{
		if (HitEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitLocation);
		}
	}

	void AEnemyCharacter::OnHitboxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{
		if (OtherActor && OtherActor != this)
		{
			IFighterInterface* CharacterRef = Cast<IFighterInterface>(OtherActor);
			FName HitboxName = OverlappedComp->GetFName();
			
			if (CharacterRef && CharacterRef->GetCombatState() == ECombatState::Attacking && DA_DMV)
			{
				if (StatsComp->Stats[Health] <= 0.0f)
				{
					Death();
					return;
				}

				BaseDamage = CharacterRef->GetDamage();
				DamageMultiplier = 1.0f;
				if (HitboxName == "HeadHitbox") DamageMultiplier = DA_DMV->HeadMultipler;
				else if (HitboxName == "TorsoHitbox") DamageMultiplier = DA_DMV->TorsoMultipler;
				else if (HitboxName.ToString().Contains("Arm")) DamageMultiplier = DA_DMV->ArmMultipler;
				else if (HitboxName.ToString().Contains("Leg")) DamageMultiplier = DA_DMV->LegMultipler;


				TotalDamage = BaseDamage * DamageMultiplier;

				StatsComp->ReduceHealth(TotalDamage);

				PlayHitReaction();

				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
					FString::Printf(TEXT("%s hit for %.1f damage!"), *HitboxName.ToString(), TotalDamage));

				OnReceivedDamageDelegate.Broadcast(3.0f);
			}

		}
	}

	float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
	{
		//TotalDamage += DamageAmount;
		//FString::Printf(TEXT("Take Damage hit for %.1f damage!"), TotalDamage);
		UE_LOG(LogTemp, Log, TEXT("Sword hit enemy part: %.1f"),TotalDamage );

		return TotalDamage;
	}



	float AEnemyCharacter::GetDamage()
	{
		return StatsComp->Stats[EStat::BaseDamage];
	}