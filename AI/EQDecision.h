// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/EQConsiderationEvaluator.h"
#include "AI/EQAITypes.h"
#include "EQDecision.generated.h"


UCLASS()
class PROJECT999_API UEQDecision : public UObject
{
	GENERATED_BODY()
	
public:
	static float CalculateScore(FUEQDecision* Decision, FUEQDecisionContext* DecisionContext, const float& Bonus, const float& MinScore)
	{
		float FinalScore = Bonus;
		for (const auto Consideration : Decision->ConditionEvaluators)
		{
			if (FinalScore <= 0.0f || FinalScore < MinScore)
			{
				return FinalScore;
			}

			float Score = UEQConsiderationEvaluator::Evaluate(*Consideration, *DecisionContext);
			float Response = FMath::Clamp(Score, 0.0f, 1.0f);
			Response = UEQConsiderationEvaluator::ApplyCurve(Consideration->Curve, Response);			
			FinalScore *= FMath::Clamp(Response, 0.0f, 1.0f);
		}
		//TODO: Apply the compensation factor
		/*
		float ModificationFactor = 1 - (1 / Decision->ConditionEvaluators.Num());
		float MakeUpValue = (1 - FinalScore) * ModificationFactor;
		FinalScore = FinalScore + MakeUpValue * FinalScore;*/
		return FinalScore;
	}

};
