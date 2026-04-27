// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/TypeFilterWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTypeFilterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	typeFilterButton->OnPressed.AddUniqueDynamic(this, &UTypeFilterWidget::OnFilterButtonPressed);
	SetFrameText();
}

//Filter by the type of the structures
void UTypeFilterWidget::OnFilterButtonPressed()
{
	
}

void UTypeFilterWidget::SetFrameText()
{
	typeTextFrame->SetText(textTypeFrame);
}

