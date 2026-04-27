// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionDataWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API USelectionDataWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> structureNameText{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> structureBorder{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> descriptionText{};

	//AQUI EL COSTE
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> buildButton{};

	//Buttons functions
	UFUNCTION()
	void OnBuildButtonPressed();

	//Structure selected for show information
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AConstructionPart> selectedStructure{};

public:
	UFUNCTION()
	void SelectStructure(FConstructionData structureData);
};
