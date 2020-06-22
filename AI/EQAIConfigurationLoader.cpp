// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQAIConfigurationLoader.h"
#include "Assets/EQJsonAsset.h"

void UEQAIConfigurationLoader::LoadAIConfiguration()
{
	check(AIJsonConfiguration);

	FUEQAIConfigurationJson AIConfigurationJson;
	AIJsonConfiguration->SerializeJson(&AIConfigurationJson);

	LoadConditionEvaluators(AIConfigurationJson.ConditionEvaluators);
	LoadDecisions(AIConfigurationJson.Decisions);
	LoadDecisionSets(AIConfigurationJson.DecisionSets);
}

void UEQAIConfigurationLoader::LoadConditionEvaluators(TArray<FUEQConditionEvaluatorJson>& ConditionEvaluatorsJson)
{
	for (const auto& ConsiderationEvaluatorJson : ConditionEvaluatorsJson)
	{
		const FUEQCurveJson& Curve = ConsiderationEvaluatorJson.Curve;
		ConditionEvaluators.Add(
			FUEQConditionEvaluator(ConsiderationEvaluatorJson.Tag, ConsiderationEvaluatorJson.FunctionName, 
				Curve.CurveType, Curve.Exponent, Curve.Slope, Curve.XShift, Curve.YShift, ConsiderationEvaluatorJson.Parameters)
		);		
	}
}

void UEQAIConfigurationLoader::LoadDecisions(TArray<FUEQDecisionJson>& DecisionsJson)
{
	for (auto& DecisionJson : DecisionsJson)
	{
		Decisions.Add(
			FUEQDecision(DecisionJson.Tag, DecisionJson.Weight, DecisionJson.ConditionEvaluators, ConditionEvaluators, 
				DecisionJson.HasTargetDependency, DecisionJson.FunctionName)
		);		
	}
}

void UEQAIConfigurationLoader::LoadDecisionSets(TArray<FUEQDecisionSetJson>& DecisionSetsJson)
{
	for (auto& DecisionSetJson : DecisionSetsJson)
	{
		FUEQDecisionSet DecisionSet = FUEQDecisionSet(DecisionSetJson.Tag, DecisionSetJson.Decisions, Decisions, DecisionSetJson.DefaultDecision);
		DecisionSets.Add(
			DecisionSet.Tag,
			DecisionSet
		);
	}
}
