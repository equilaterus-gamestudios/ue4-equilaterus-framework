// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EQDialogComponent.h"
#include "Assets/EQJsonAsset.h"
#include "Utility/EQFunctionBlueprintLibrary.h"
#include "Utility/EQTMapBlueprintLibrary.h"
#include "Core/EQCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UI/EQDialogWidget.h"

// Sets default values for this component's properties
UEQDialogComponent::UEQDialogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//DialogJson = CreateDefaultSubobject<UEQJsonAsset>(TEXT())
	// ...
}


// Called when the game starts
void UEQDialogComponent::BeginPlay()
{
	Super::BeginPlay();
	
	DialogJson->SerializeJson(&DialogScript);
	// ...
	
}


// Called every frame
void UEQDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEQDialogComponent::PlayDialog()
{
	check(Widget);

	//TODO REMOVE THIS DUPLICATED CODE
	//Reached the end of the dialog
	if (MemoryPosition < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("End of the dialog"));
		Character->RemoveDialogWidget();
		return;
	}

	int MaxPosition = DialogScript.Statements.Num();
	for (; MemoryPosition < MaxPosition; ++MemoryPosition)
	{
		FUEQDialogStatement* CurrentStatement = &DialogScript.Statements[MemoryPosition];
		//CurrentStatement->FunctionName
		const FName& FunctionName = CurrentStatement->FunctionName;
		FUEQDialogStatementContainer StatementContainer = FUEQDialogStatementContainer(CurrentStatement);
		bool bStopableAction = UEQFunctionBlueprintLibrary::ExecuteFunctionByName<bool, FUEQDialogStatementContainer>(FunctionName, this, &StatementContainer);

		//If the current action is waiting interaction, point to the next action, and wait until other method
		//continue the dialog
		if (bStopableAction)
		{
			++MemoryPosition;
			break;
		}

		//Reached the end of the dialog
		if (MemoryPosition < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("End of the dialog"));
			Character->RemoveDialogWidget();
			break;
		}
	}
}

void UEQDialogComponent::PlayDialog(AActor* OtherActor)
{
	Character = Cast<AEQCharacter>(OtherActor);
	check(Character);
	Widget = Cast<UEQDialogWidget>(Character->DisplayDialogWidget(this));

	MemoryPosition = 0;			

	PlayDialog();
}

void UEQDialogComponent::ContinueDialog(bool bFromTheBegining)
{
	if (bFromTheBegining)
	{
		MemoryPosition = 0;
	}

	PlayDialog();
}

void UEQDialogComponent::ContinueDialogFrom(int Line)
{
	MemoryPosition = Line;

	PlayDialog();
}

bool UEQDialogComponent::SetDialog(FUEQDialogStatementContainer& InDialogStatement)
{
	FUEQDialogStatement* CurrentStatement = InDialogStatement.DialogStatement;
	FString Title = UEQTMapBlueprintLibrary::GetStringParameter(CurrentStatement->Parameters, FName("Title"));
	FString Text = UEQTMapBlueprintLibrary::GetStringParameter(CurrentStatement->Parameters, FName("Text"));
	Widget->NativeOnSetDialog(Title, Text);
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *Title, *Text);
	return false;
}

bool UEQDialogComponent::SetOption(FUEQDialogStatementContainer& InDialogStatement)
{	
	FUEQDialogStatement* CurrentStatement = InDialogStatement.DialogStatement;
	FString Text = UEQTMapBlueprintLibrary::GetStringParameter(CurrentStatement->Parameters, FName("Text"));
	int Line = UEQTMapBlueprintLibrary::HasParameter(CurrentStatement->Parameters, FName("JumpTo"))
		? UEQTMapBlueprintLibrary::GetIntParameter(CurrentStatement->Parameters, FName("JumpTo"))
		: -2;

	Widget->NativeOnSetOption(Text, Line);
	UE_LOG(LogTemp, Warning, TEXT("%s -> Jump to: %i"), *Text, Line);
	return false;
}

bool UEQDialogComponent::WaitInteraction(FUEQDialogStatementContainer& InDialogStatement)
{
	UE_LOG(LogTemp, Warning, TEXT("Waiting interaction..."));
	Widget->NativeOnWaitInteraction();
	return true;
}

bool UEQDialogComponent::WaitOptionSelection(FUEQDialogStatementContainer& InDialogStatement)
{
	UE_LOG(LogTemp, Warning, TEXT("Waiting option selection..."));
	Widget->NativeOnWaitOptionSelection();
	return true;
}

bool UEQDialogComponent::JumpTo(FUEQDialogStatementContainer& InDialogStatement)
{
	FUEQDialogStatement* CurrentStatement = InDialogStatement.DialogStatement;
	int Line = UEQTMapBlueprintLibrary::GetIntParameter(CurrentStatement->Parameters, FName("Line"));
	JumpTo(Line);
	return false;
}

bool UEQDialogComponent::ExecuteCommand(FUEQDialogStatementContainer& InDialogStatement)
{
	FUEQDialogStatement* CurrentStatement = InDialogStatement.DialogStatement;
	FString Variable = UEQTMapBlueprintLibrary::GetStringParameter(CurrentStatement->Parameters, FName("Variable"));
	FString Value = UEQTMapBlueprintLibrary::GetStringParameter(CurrentStatement->Parameters, FName("Value"));

	UE_LOG(LogTemp, Warning, TEXT("%s changes to %s"), *Variable, *Value);
	return false;
}

