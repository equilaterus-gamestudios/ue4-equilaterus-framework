// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EQDialogWidget.generated.h"


USTRUCT(BlueprintType)
struct FUEQDialogOption
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Text;

	UPROPERTY(BlueprintReadWrite)
	int Line;

	FUEQDialogOption() {}
	FUEQDialogOption(FString& InText, int& InLine)
	{
		Text = InText;
		Line = InLine;
	}
};



class UEQDialogComponent;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class PROJECT999_API UEQDialogWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	UEQDialogComponent* DialogComponent;

	UPROPERTY(BlueprintReadWrite)
	TArray<FUEQDialogOption> Options;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetDialog(const FString& Title, const FString& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetOption(const FUEQDialogOption& DialogOption);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWaitInteraction();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWaitOptionSelection();

public:
	virtual void NativeOnSetDialog(FString& Title, FString& Text);

	virtual void NativeOnSetOption(FString& Options, int& Line);

	virtual void NativeOnWaitInteraction();

	virtual void NativeOnWaitOptionSelection();

	FORCEINLINE void SetDialogComponent(UEQDialogComponent* InDialogComponent)
	{
		DialogComponent = InDialogComponent;
	}
};
