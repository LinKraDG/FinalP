// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/PlayerCharacter.h"
#include "Structs/CraftIngredient.h"
#include "Structs/InventoryItem.h"
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
	FCraftIngredient bpIngredient;
	bool hasMaterials;

	UPROPERTY(EditAnywhere)
	int itemID;

	void Refresh();

	void OnInventoryItemChanged(FInventoryItem Item);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInventoryComponent> inventory;
	
protected:
	virtual void NativeDestruct() override;
	
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
	void SetData(FItemData ingredient, int amount);

	UFUNCTION()
	bool GetHasMaterials();

	UFUNCTION()
	int GetItemID();
	
	UFUNCTION()
	int GetCost();
	
};
