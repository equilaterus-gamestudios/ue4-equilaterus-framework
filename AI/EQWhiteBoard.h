// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EQWhiteBoard.generated.h"

class AController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT999_API UEQWhiteBoard : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEQWhiteBoard();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	AController* Instigator;

	UPROPERTY(BlueprintReadWrite)
	TArray<UObject*> Targets;

	UPROPERTY(BlueprintReadWrite)
	bool HasLineOfSightToEnemy;

	UPROPERTY(BlueprintReadWrite)
	float LastStimulusTime;

	UPROPERTY(BlueprintReadWrite)
	float CurrentTime;
	
	UPROPERTY(BlueprintReadWrite)
	FVector LastKnownEnemyLocation;

	UPROPERTY(BlueprintReadWrite)
	bool HasReachedLastKnownEnemyLocation;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAlert;
};
