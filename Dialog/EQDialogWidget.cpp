// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EQDialogWidget.h"

void UEQDialogWidget::NativeOnSetDialog(FString& Title, FString& Text)
{
	OnSetDialog(Title, Text);
}

void UEQDialogWidget::NativeOnSetOption(FString& Option, int& Line)
{
	FUEQDialogOption DialogOption = FUEQDialogOption(Option, Line);
	OnSetOption(DialogOption);
}

void UEQDialogWidget::NativeOnWaitInteraction()
{
	OnWaitInteraction();
}

void UEQDialogWidget::NativeOnWaitOptionSelection()
{
	OnWaitOptionSelection();
}
