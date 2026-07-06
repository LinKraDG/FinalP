// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	currentStamina = maxStamina;
}

void UStaminaComponent::UpdateStamina(float deltaTime, bool bSprinting)
{
	if (bSprinting && currentStamina > 0.f){
		currentStamina = FMath::Max(0.f, currentStamina - staminaRate * deltaTime);

		if (currentStamina <= 0.f){
			bExhausted = true;
		}
	}
	else if (currentStamina < maxStamina){
		currentStamina = FMath::Min(maxStamina, currentStamina + regenStaminaRate * deltaTime);

		if (bExhausted && currentStamina > 0.f){
			bExhausted = false;
		}
	}
}

