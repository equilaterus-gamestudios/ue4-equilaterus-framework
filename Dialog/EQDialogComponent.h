// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EQDialogComponent.generated.h"

class UEQJsonAsset;
class AEQCharacter;
class UEQDialogWidget;


USTRUCT()
struct FUEQDialogStatement
{
	GENERATED_BODY()

	UPROPERTY()
	FName FunctionName;
		
	UPROPERTY()
	TMap<FName, FString> Parameters;
};

USTRUCT()
struct FUEQDialogScript
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FUEQDialogStatement> Statements;
};


USTRUCT()
struct FUEQDialogStatementContainer
{
	GENERATED_BODY()

	FUEQDialogStatement* DialogStatement;

	FUEQDialogStatementContainer() {}
	FUEQDialogStatementContainer(FUEQDialogStatement* InDialogStatement)
	{
		DialogStatement = InDialogStatement;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT999_API UEQDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEQDialogComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FUEQDialogScript DialogScript;

	UPROPERTY(EditAnywhere)
	UEQJsonAsset* DialogJson;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	int MemoryPosition;

	UEQDialogWidget* Widget;

	AEQCharacter* Character;

	void PlayDialog();

	UFUNCTION(BlueprintCallable)
	void PlayDialog(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void ContinueDialog(bool bFromTheBegining);

	UFUNCTION(BlueprintCallable)
	void ContinueDialogFrom(int Line);

	UFUNCTION()
	bool SetDialog(FUEQDialogStatementContainer& InDialogStatement);

	UFUNCTION()
	bool SetOption(FUEQDialogStatementContainer& InDialogStatement);

	UFUNCTION()
	bool WaitInteraction(FUEQDialogStatementContainer& InDialogStatement);

	UFUNCTION()
	bool WaitOptionSelection(FUEQDialogStatementContainer& InDialogStatement);

	UFUNCTION()
	bool JumpTo(FUEQDialogStatementContainer& InDialogStatement);

	FORCEINLINE void JumpTo(int Line)
	{
		MemoryPosition = Line;
	}

	UFUNCTION()
	bool ExecuteCommand(FUEQDialogStatementContainer& InDialogStatement);

		
};
