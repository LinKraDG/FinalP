// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/StructuresClassify.h"
#include "TypeFilterWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UTypeFilterWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	EStructureType type;

protected:
	virtual void NativeOnInitialized() override;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class APlayerHUD> hud{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> typeFrame{};

	UPROPERTY(EditAnywhere)
	UTexture2D* imageTypeFrame;

	UPROPERTY(EditAnywhere)
	FText textTypeFrame;

	UPROPERTY(EditAnywhere)
	FText textType;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> typeTextFrame;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> typeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> typeFilterButton{};

	//Buttons functions
	UFUNCTION()
	void OnFilterButtonPressed();

	UFUNCTION()
	void SetFrameText();
	
};
