// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidget.h"
#include "Components/ProgressBar.h"

void UPlayerWidget::SetStaminaPercent(float percent)
{
	if (IsValid(staminaBar)){
		staminaBar->SetPercent(FMath::Clamp(percent, 0.0f, 1.0f));
	}
}