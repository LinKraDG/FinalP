// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

private:

public:

protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> resumeButton{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> exitButton{};

	UFUNCTION()
	void OnResumeButtonPressed();

	UFUNCTION()
	void OnExitButtonPressed();
	
};
