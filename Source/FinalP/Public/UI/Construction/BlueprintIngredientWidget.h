// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CraftIngredient.h"
#include "BlueprintIngredientWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UBlueprintIngredientWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	FItemData ingredientsData;

	UPROPERTY(EditAnywhere)
	int itemID;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> imageFrame{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> nameText{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> actAmountText{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> costText{};

public:
	UFUNCTION()
	void SetData(FCraftIngredient ingredient);
};
