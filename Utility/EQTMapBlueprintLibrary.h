// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EQTMapBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT999_API UEQTMapBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FORCEINLINE bool HasParameter(TMap<FName, FString>& Map, FName&& PropertyName)
	{
		return Map.Contains(PropertyName);
	}

	static FORCEINLINE FString GetStringParameter(TMap<FName, FString>& Map, FName&& PropertyName)
	{
		check(Map.Contains(PropertyName));
		return Map[PropertyName];
	}

	static FORCEINLINE int GetIntParameter(TMap<FName, FString>& Map, FName&& PropertyName)
	{
		check(Map.Contains(PropertyName));
		return FCString::Atoi(*Map[PropertyName]);
	}

	static FORCEINLINE float GetFloatParameter(TMap<FName, FString>& Map, FName&& PropertyName)
	{
		check(Map.Contains(PropertyName));
		return FCString::Atof(*Map[PropertyName]);
	}

	static FORCEINLINE bool GetBoolParameter(TMap<FName, FString>& Map, FName&& PropertyName)
	{
		check(Map.Contains(PropertyName));
		return Map[PropertyName].ToBool();
	}	
};
