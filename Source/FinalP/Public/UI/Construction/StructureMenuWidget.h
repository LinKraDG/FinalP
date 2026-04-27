// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructureMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UStructureMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class UPanelWidget> structureBlueprintsPanel{};

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class UPanelWidget> structureTypeFilterPanel{};

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class UPanelWidget> structureSortFilterPanel{};
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class USelectionDataWidget> selectionWidget{};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUnicStructure> structuresWidget;

	TObjectPtr<class UDataTable> dataTable = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	UPanelWidget* GetStructurePanel();

	UFUNCTION()
	void SlotCreation(UDataTable* table, TArray<FName> names);

	UFUNCTION()
	void CreateStructureSlot(FName row);

	UFUNCTION()
	USelectionDataWidget* GetSelectionWidget();

};
