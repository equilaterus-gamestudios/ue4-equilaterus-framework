// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"
#include "AI/EQAITypes.h"
#include "EQAIConfigurationLoader.generated.h"


USTRUCT()
struct FUEQCurveJson
{
	GENERATED_BODY()

	UPROPERTY()
	FName CurveType;

	UPROPERTY()
	float Exponent;

	UPROPERTY()
	float Slope;

	UPROPERTY()
	float XShift;

	UPROPERTY()
	float YShift;
};

USTRUCT()
struct FUEQConditionEvaluatorJson
{
	GENERATED_BODY()

	UPROPERTY()
	FName Tag;

	UPROPERTY()
	FName FunctionName;

	UPROPERTY()
	FUEQCurveJson Curve;

	UPROPERTY()
	TMap<FName, FString> Parameters;	
};

USTRUCT()
struct FUEQDecisionJson
{
	GENERATED_BODY()

	UPROPERTY()
	FName Tag;

	UPROPERTY()
	FName DecisionName;

	UPROPERTY()
	float Weight;

	UPROPERTY()
	bool HasTargetDependency;

	UPROPERTY()
	FName FunctionName;
		
	UPROPERTY()
	TArray<FName> ConditionEvaluators;
};

USTRUCT()
struct FUEQDecisionSetJson
{
	GENERATED_BODY()

	UPROPERTY()
	FName Tag;
	
	UPROPERTY()
	TArray<FName> Decisions;

	UPROPERTY()
	FName DefaultDecision;
};

USTRUCT()
struct FUEQAIConfigurationJson
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FUEQConditionEvaluatorJson> ConditionEvaluators;

	UPROPERTY()
	TArray<FUEQDecisionJson> Decisions;

	UPROPERTY()
	TArray<FUEQDecisionSetJson> DecisionSets;
};


class UEQJsonAsset;

UCLASS(Blueprintable)
class PROJECT999_API UEQAIConfigurationLoader : public UObject
{
	GENERATED_BODY()

private:	
	UPROPERTY()
	TArray<FUEQConditionEvaluator> ConditionEvaluators;

	UPROPERTY()
	TArray<FUEQDecision> Decisions;

	UPROPERTY()
	TMap<FName, FUEQDecisionSet> DecisionSets;
	
	UPROPERTY(EditDefaultsOnly)
	UEQJsonAsset* AIJsonConfiguration;
		
	void LoadConditionEvaluators(TArray<FUEQConditionEvaluatorJson>& ConditionEvaluatorsJson);

	void LoadDecisions(TArray<FUEQDecisionJson>& DecisionsJson);

	void LoadDecisionSets(TArray<FUEQDecisionSetJson>& DecisionSetsJson);

public:
	UFUNCTION(BlueprintCallable)
	void LoadAIConfiguration();

	FORCEINLINE FUEQDecisionSet* GetDecisionSetByTag(FName& Tag)
	{
		return &DecisionSets[Tag];		
	}

	template<typename T>
	static bool DeserializeJsonFile(FString& FilePath, T& OutTObject)
	{
		FString FileData = "";

		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("FILE NOT FOUND"));
			return false;
		}

		FFileHelper::LoadFileToString(FileData, *FilePath);
		return FJsonObjectConverter::JsonObjectStringToUStruct(FileData, &OutTObject, 0, 0);
	}
};
