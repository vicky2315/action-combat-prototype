// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATPROJECT_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnyWhere)
	FName StartSocket;

	UPROPERTY(EditAnyWhere)
	FName EndSocket;

	UPROPERTY(EditAnyWhere)
	FName SRotation;

	FVector StartSocketLocation, EndSocketLocation;

	FQuat SocketRotation;

	TArray<FHitResult> OutResults;
	FVector BoxSize;
	double WeaponDistance;

	UPROPERTY(EditAnywhere)
	double BoxCollisionLength{ 30.0 };

	UPROPERTY(EditAnywhere)
	bool bDebugMode{ false };

	UPROPERTY(Transient)
	TArray<AActor*> ActorsDamaged;

	UNiagaraComponent* SpawnedSystem;

	APlayerController* PC;



public:	
	// Sets default values for this component's properties
	UTraceComponent();

	UPROPERTY(EditAnywhere)
	bool bIsAttacking{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* HitNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> HitCameraShake;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor* AttachedActor;

	//FDamageEvent DamageDone;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

		
};
