// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/BPI_Enemy.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/FighterInterface.h"
#include "DataAssets/DamageMultiplier/DamageMultiplierValues.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.generated.h"

class AEnemyAIController;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnReceivedDamageSignature,
	AEnemyCharacter,
	OnReceivedDamageDelegate,
	float, Duration
);

UCLASS()
class MELEECOMBATPROJECT_API AEnemyCharacter : public ACharacter, public IBPI_Enemy, public IFighterInterface
{
	GENERATED_BODY()

	class UBlackboardComponent* BlackboardComp;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EEnemyState> InitialState{ EEnemyState::NoMovement };

	UPROPERTY(BlueprintReadWrite, Editanywhere)
	class UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere)
	UAnimMontage* LightAttackMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HeavyAttackMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitReactionMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DedMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	bool bTrainingDummyMode;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere)
	float TotalDamage;

	UPROPERTY(EditAnywhere)
	UDamageMultiplierValues* DA_DMV;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTraceComponent* LeftTraceComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTraceComponent* RightTraceComp;

	//Hitbox for body parts.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hitboxes")
	UCapsuleComponent* HeadHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hitboxes")
	UCapsuleComponent* TorsoHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hitboxes")
	UCapsuleComponent* LeftArmHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hitboxes")
	UCapsuleComponent* RightArmHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hitboxes")
	UCapsuleComponent* LeftLegHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hitboxes")
	UCapsuleComponent* RightLegHitbox;

	UPROPERTY(BlueprintAssignable)
	FOnReceivedDamageSignature OnReceivedDamageDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float BaseDamage, DamageMultiplier { 0.0f };

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerDetected(AActor* DetectedActor);

	void LightAttack();

	void HeavyAttack();

	void PlayHitReaction();

	void Death();

	void PlayHitEffect(const FVector& HitLocation);

	virtual float GetDamage() override;

	UFUNCTION(BlueprintCallable)
	void HandleDamage(float Damage, FVector HitLocation, FVector InstigatorLocation);

	void ApplyKnockback(const FVector& HitDirection, float Strength);

	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

};