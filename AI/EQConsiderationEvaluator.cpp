// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQConsiderationEvaluator.h"
#include "Core/EQCharacter.h"
#include "Components/EQHealthComponent.h"
#include "AI/EQWhiteBoard.h"
#include "AI/EQAITypes.h"
#include "AI/EQBaseNPCCharacter.h"
#include "GameFramework/Controller.h"
#include "Utility/EQCurveBlueprintLibrary.h"
#include "Utility/EQFunctionBlueprintLibrary.h"
#include "Utility/EQTMapBlueprintLibrary.h"


const FName UEQConsiderationEvaluator::FunctionPrefix = FName("Evaluate_");

float UEQConsiderationEvaluator::ApplyCurve(FUEQCurve& Curve, float& Score)
{
	switch (Curve.CurveType)
	{
	case EEQCurveTypeEnum::EQCT_Polinomial:
		return UEQCurveBlueprintLibrary::CalculatePolinomialCurve(Curve.Exponent, Curve.Slope,
			Curve.XShift, Curve.YShift, Score);
	case EEQCurveTypeEnum::EQCT_Gaussian:
		return UEQCurveBlueprintLibrary::CalculateGaussianCurve(Curve.Exponent, Curve.Slope,
			Curve.XShift, Curve.YShift, Score);
	default:
		checkNoEntry()
		break;
	}
	return 0.0f;
}

float UEQConsiderationEvaluator::Evaluate(FUEQConditionEvaluator& ConditionEvaluator, FUEQDecisionContext& Context)
{
	const FString FunctionString = FunctionPrefix.ToString() + ConditionEvaluator.EvaluatorName.ToString();
	const FName FunctionName = FName(*FunctionString);
	// The only way to invoke the function with the ue framework is through an object. So get the default object
	UObject* DefaultObject = UEQConsiderationEvaluator::StaticClass()->GetDefaultObject();
	FUEQConditionStruct ConditionParameters = FUEQConditionStruct(&ConditionEvaluator, &Context);

	return UEQFunctionBlueprintLibrary::ExecuteFunctionByName<float, FUEQConditionStruct>(
		FunctionName, DefaultObject, &ConditionParameters
	);
}

float UEQConsiderationEvaluator::Evaluate_None(FUEQConditionStruct& ConditionParameters)
{
	return 0.8f;
}

float UEQConsiderationEvaluator::Evaluate_ConsiderationMyHealth(FUEQConditionStruct& ConditionParameters)
{
	const FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;

	AController* Controller = Context->Instigator;
	AEQCharacter* Character = Cast<AEQCharacter>(Controller->GetPawn());

	if (!Character)
		return 0.0f;

	const UEQHealthComponent* HealthComponent = Character->GetHealthComponent();
	return HealthComponent->GetHealth() / HealthComponent->GetHealthMax();
}

float UEQConsiderationEvaluator::Evaluate_ConsiderationTargetHealth(FUEQConditionStruct& ConditionParameters)
{
	const FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;

	UObject* Target = Context->Target;
	AActor* ActorTarget = Cast<AActor>(Target);

	if (!ActorTarget)
		return 0.0f;

	TSubclassOf<UEQHealthComponent> SubclassOfHealthComponent = TSubclassOf<UEQHealthComponent>(UEQHealthComponent::StaticClass());
	UActorComponent* Component = ActorTarget->GetComponentByClass(SubclassOfHealthComponent);
	const UEQHealthComponent* HealthComponent = Cast<UEQHealthComponent>(Component);

	if (!HealthComponent)
		return 0.0f;

	return HealthComponent->GetHealth() / HealthComponent->GetHealthMax();
}

float UEQConsiderationEvaluator::Evaluate_LineOfSight(FUEQConditionStruct& ConditionParameters)
{
	const FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;

	return Context->WhiteBoard->HasLineOfSightToEnemy
		? 1.0f
		: 0.0f;
}

float UEQConsiderationEvaluator::Evaluate_StimulusTime(FUEQConditionStruct& ConditionParameters)
{
	FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;

	const float CurrentTime = Context->WhiteBoard->CurrentTime;
	const float LastStimulusTime = Context->WhiteBoard->LastStimulusTime;
	const float MaxStimulusTime = UEQTMapBlueprintLibrary::GetFloatParameter(ConsiderationConfiguration->Parameters, FName("MaxTime"));
	
	return (CurrentTime - LastStimulusTime) / MaxStimulusTime;
}

float UEQConsiderationEvaluator::Evaluate_TargetAffiliation(FUEQConditionStruct& ConditionParameters)
{
	const FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;

	const APawn* TargetPawn = Cast<APawn>(Context->Target);
	if (!TargetPawn)
	{
		return 0.0f;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(TargetPawn->GetController());

	return PlayerController
		? 1.0f
		: 0.0f;
}

float UEQConsiderationEvaluator::Evaluate_TargetDistance(FUEQConditionStruct& ConditionParameters)
{
	FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;
	
	const APawn* TargetPawn = Cast<APawn>(Context->Target);
	const FVector ActorLocation = Context->Instigator->GetPawn()->GetActorLocation();
	const float Distance = FVector::Distance(TargetPawn->GetActorLocation(), ActorLocation);

	float RangeMin = UEQTMapBlueprintLibrary::GetFloatParameter(ConsiderationConfiguration->Parameters, FName("MinRange"));
	float RangeMax = UEQTMapBlueprintLibrary::GetFloatParameter(ConsiderationConfiguration->Parameters, FName("MaxRange"));	

	return FMath::Clamp((Distance - RangeMin) / (RangeMax - RangeMin), 0.0f, 1.0f);
}

float UEQConsiderationEvaluator::Evaluate_HasReachedLastKnownEnemyLocation(FUEQConditionStruct& ConditionParameters)
{
	const FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;

	return Context->WhiteBoard->HasReachedLastKnownEnemyLocation
		? 1.0f
		: 0.0f;
}

float UEQConsiderationEvaluator::Evaluate_HasPatrolPath(FUEQConditionStruct& ConditionParameters)
{
	const FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;

	AEQBaseNPCCharacter* NPCCharacter = Cast<AEQBaseNPCCharacter>(Context->Instigator->GetPawn());

	return NPCCharacter->bHasPatrolPath
		? 1.0f
		: 0.0f;
}

float UEQConsiderationEvaluator::Evaluate_IsAlert(FUEQConditionStruct& ConditionParameters)
{
	const FUEQConditionEvaluator* ConsiderationConfiguration = ConditionParameters.ConditionEvaluator;
	const FUEQDecisionContext* Context = ConditionParameters.DecisionContext;
	
	return Context->WhiteBoard->bIsAlert
		? 1.0f
		: 0.0f;
}
