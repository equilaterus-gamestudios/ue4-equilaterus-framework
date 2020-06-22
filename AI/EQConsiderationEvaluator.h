// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/EQAITypes.h"
#include "EQConsiderationEvaluator.generated.h"


USTRUCT()
struct FUEQConditionStruct
{
	GENERATED_BODY()

	struct FUEQConditionEvaluator* ConditionEvaluator;

	struct FUEQDecisionContext* DecisionContext;

	FUEQConditionStruct(FUEQConditionEvaluator* InConditionEvaluator, FUEQDecisionContext* InDecisionContext)
		: ConditionEvaluator(InConditionEvaluator), DecisionContext(InDecisionContext)
	{
	}

	FUEQConditionStruct() {}

};

/**
 * 
 */
UCLASS()
class PROJECT999_API UEQConsiderationEvaluator : public UObject
{
	GENERATED_BODY()

public:
	static const FName FunctionPrefix;

	static float ApplyCurve(FUEQCurve& Curve, float& Score);

	static float Evaluate(FUEQConditionEvaluator& ConditionEvaluator, struct FUEQDecisionContext& Context);

	UFUNCTION()
	static float Evaluate_None(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_ConsiderationMyHealth(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_ConsiderationTargetHealth(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_LineOfSight(FUEQConditionStruct& ConsiderationParameters);
	
	UFUNCTION()
	static float Evaluate_StimulusTime(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_TargetAffiliation(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_TargetDistance(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_HasReachedLastKnownEnemyLocation(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_HasPatrolPath(FUEQConditionStruct& ConsiderationParameters);

	UFUNCTION()
	static float Evaluate_IsAlert(FUEQConditionStruct& ConsiderationParameters);
};
