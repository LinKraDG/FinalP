// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CraftIngredient.h"
#include "Structs/InventoryItem.h"
#include "SelectionDataWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API USelectionDataWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInventoryComponent> inventory{};
	
	void Refresh();

	void OnInventoryItemChanged(FInventoryItem Item);
	
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> structureNameText{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> structureBorder{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> descriptionText{};

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class UPanelWidget> structureCostPanel{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> buildButton{};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UBlueprintIngredientWidget> costWidget;

	//Buttons functions
	UFUNCTION()
	void OnBuildButtonPressed();

	//Structure selected for show information
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AConstructionPart> selectedStructure{};

public:
	UFUNCTION()
	void SelectStructure(FConstructionData structureData);

	UFUNCTION()
	void CreateIngredientSlot(FCraftIngredient ingredient);

	UFUNCTION()
	void RemoveSlots();

	UFUNCTION()
	bool CheckHasMaterials();
};
