// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALP_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float maxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float staminaRate = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float regenStaminaRate = 20.f;

	UFUNCTION(BlueprintCallable)
	void UpdateStamina(float deltaTime, bool bSprinting);

	UFUNCTION()
	bool CanSprint() const { return !bExhausted && currentStamina > 0.f; }

	UFUNCTION()
	float GetStamina() const { return currentStamina; }

	UFUNCTION()
	float GetStaminaPercent() const { return maxStamina > 0.f ? currentStamina / maxStamina : 0.f; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float currentStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	bool bExhausted = false;

};
