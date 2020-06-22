// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EQAITypes.generated.h"

USTRUCT()
struct FUEQDecisionContext
{
	GENERATED_BODY()

	class AController* Instigator;

	UObject* Target;

	FUEQDecision* Decision;

	class UEQWhiteBoard* WhiteBoard;

	float Score;

	FUEQDecisionContext() : Score(0.0f) { }

	FUEQDecisionContext(AController* InInstigator, UObject* InTarget, FUEQDecision* InDecision, UEQWhiteBoard* InWhiteBoard)
		: Score(0.0f)
	{
		Instigator = InInstigator;
		Target = InTarget;
		Decision = InDecision;
		WhiteBoard = InWhiteBoard;
	}
};


UENUM(BlueprintType)
enum class EEQCurveTypeEnum : uint8
{
	EQCT_Polinomial				UMETA(DisplayName = "Polinomial"),
	EQCT_Gaussian 				UMETA(DisplayName = "Gaussian")
};


USTRUCT()
struct FUEQCurve
{
	GENERATED_BODY()

	UPROPERTY()
	EEQCurveTypeEnum CurveType;

	UPROPERTY()
	float Exponent;

	UPROPERTY()
	float Slope;

	UPROPERTY()
	float XShift;

	UPROPERTY()
	float YShift;

	FUEQCurve() {}

	FUEQCurve(FName InCurveType, float InExponent, float InSlope, float InXShift, float InYShift)
		: Exponent(InExponent), Slope(InSlope), XShift(InXShift), YShift(InYShift)
	{
		if (InCurveType == "Polinomial")
			CurveType = EEQCurveTypeEnum::EQCT_Polinomial;
		else if (InCurveType == "Gaussian")
			CurveType = EEQCurveTypeEnum::EQCT_Gaussian;
		else
			checkNoEntry();
	}

};


USTRUCT()
struct FUEQConditionEvaluator
{
	GENERATED_BODY()

	UPROPERTY()
	FName Tag;

	UPROPERTY()
	FName EvaluatorName;

	UPROPERTY()
	FUEQCurve Curve;

	UPROPERTY()
	TMap<FName, FString> Parameters;
	
	FUEQConditionEvaluator() {}

	FUEQConditionEvaluator(FName InTag, FName InEvaluatorName, FName InCurveType,
		float InExponent, float InSlope, float InXShift, float InYShift, TMap<FName, FString> InParameters) 
		:  Tag(InTag), EvaluatorName(InEvaluatorName)
	{
		Curve = FUEQCurve(InCurveType, InExponent, InSlope, InXShift, InYShift);
		Parameters = TMap<FName, FString>(InParameters);
	}

};


USTRUCT()
struct FUEQDecision
{
	GENERATED_BODY()

	FName Tag;

	float Weight;

	bool bHasTargetDependency;

	FName FunctionName;

	TArray<FUEQConditionEvaluator*> ConditionEvaluators;

	FORCEINLINE FUEQDecisionContext CreateDecisionContext(AController* InInstigator, UObject* InTarget, UEQWhiteBoard* InWhiteBoard)
	{
		return FUEQDecisionContext(InInstigator, InTarget, this, InWhiteBoard);
	}

	FUEQDecision() {}

	FUEQDecision(FName InTag, float InWeight, TArray<FName>& ConditionEvaluatorTags, TArray<FUEQConditionEvaluator>& AllConditionEvaluators,
		bool bInHasTargetDependency, FName InFunctionName)
		: Tag(InTag), Weight(InWeight), bHasTargetDependency(bInHasTargetDependency), FunctionName(InFunctionName)
	{
		for (const auto& ConditionEvaluatorTag : ConditionEvaluatorTags)
		{
			FUEQConditionEvaluator* ConditionEvaluator =
				AllConditionEvaluators.FindByPredicate([ConditionEvaluatorTag](const FUEQConditionEvaluator& element) {
					return element.Tag == ConditionEvaluatorTag;
				});
			if (ConditionEvaluator)
			{
				ConditionEvaluators.Add(ConditionEvaluator);
			}
		}
	}
};


USTRUCT()
struct FUEQDecisionSet
{
	GENERATED_BODY()

	FName Tag;
	
	TArray<FUEQDecision*> Decisions;

	FUEQDecision* DefaultDecision;

	FUEQDecisionSet() {}

	FUEQDecisionSet(FName InTag, TArray<FName>& DecisionTags, TArray<FUEQDecision>& AllDecisions, FName& InDefaultDecision)
		: Tag(InTag)
	{
		DefaultDecision = AllDecisions.FindByPredicate([InDefaultDecision](const FUEQDecision& element) {
			return element.Tag == InDefaultDecision;
		});

		check(DefaultDecision);
		check(!DefaultDecision->bHasTargetDependency);

		for (const auto& DecisionTag : DecisionTags)
		{
			FUEQDecision* Decision =
				AllDecisions.FindByPredicate([DecisionTag](const FUEQDecision& element) {
					return element.Tag == DecisionTag;
				});
			if (Decision)
			{
				Decisions.Add(Decision);
			}
		}
	}

};


USTRUCT(BlueprintType)
struct FUEQDecisionBoard
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	AController* Instigator;

	UPROPERTY(BlueprintReadWrite)
	TArray<UObject*> Targets;

	FUEQDecisionBoard() { }

	FUEQDecisionBoard(AController* InInstigator)
	{
		Instigator = InInstigator;		
	}
};
