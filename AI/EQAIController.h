// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/EQAITypes.h"
#include "AI/EQWhiteBoard.h"
#include "EQAIController.generated.h"


UENUM(BlueprintType)
enum class EEQNPCStateEnum : uint8
{
	EQNPCS_Idle				UMETA(DisplayName = "Idle"),
	EQNPCS_Fire				UMETA(DisplayName = "Fire"),
	EQNPCS_Move				UMETA(DisplayName = "Move")
};

class UEQWhiteBoard;
class AEQBaseNPCCharacter;

UCLASS()
class PROJECT999_API AEQAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEQAIController();

protected:
	static const FName FunctionPrefix;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName DefaultsDecisionSet;

	FUEQDecisionSet* DecisionSet;

	FUEQDecisionContext CurrentDecision;

	FTimerHandle TimeHandler_TickBrain;

	UPROPERTY()
	UEQWhiteBoard* WhiteBoard;

	virtual void BeginPlay() override;

	virtual void TickBrain();
	
	FUEQDecisionContext GetBestDecision();

	EEQNPCStateEnum CurrentNPCState;

	AEQBaseNPCCharacter* ControlledNPC;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UEQWhiteBoard* GetWhiteBoard()
	{
		return WhiteBoard;
	}
	
	UFUNCTION(BlueprintCallable)
	void AddTarget(UObject* Target)
	{
		WhiteBoard->Targets.AddUnique(Target);
	}

	UFUNCTION(BlueprintCallable)
	void RemoveTarget(UObject* Target)
	{
		WhiteBoard->Targets.Remove(Target);
	}

	/*
		Decisions
	*/
protected:

	void SetNPCState(EEQNPCStateEnum InNPCState);

	void Excecute_Decision();

	UFUNCTION()
	void Decision_MoveTo();

	UFUNCTION()
	void Decision_SaySomething();

	UFUNCTION()
	void Decision_Idle();
	
	UFUNCTION()
	void Decision_Patrol();

	UFUNCTION()
	void Decision_FreePatrol();

	UFUNCTION()
	void Decision_MoveTowardsEnemy();

	UFUNCTION()
	void Decision_Fire();

	UFUNCTION()
	void Decision_SearchEnemy();

	UFUNCTION()
	void Decision_MoveToLastKnownEnemyLocation();
};
