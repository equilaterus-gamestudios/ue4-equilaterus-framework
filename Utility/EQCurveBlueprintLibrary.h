// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EQCurveBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT999_API UEQCurveBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	UFUNCTION()
	static FORCEINLINE float CalculatePolinomialCurve(float& Exponent, float& Slope, float& XShift, float& YShift, float& X) 
	{
		return Slope * FMath::Pow(X, Exponent) - YShift + XShift;
	}

	UFUNCTION()
	static FORCEINLINE float CalculateGaussianCurve(float& Exponent, float& Slope, float& XShift, float& YShift, float& X)
	{
		return Exponent * FMath::Exp(-Slope * FMath::Pow((X - XShift) / YShift, 2));
	}
	
};
