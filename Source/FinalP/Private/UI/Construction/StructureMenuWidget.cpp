// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/StructureMenuWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/ConstructionComponent.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Structs/ConstructionData.h"
#include "Construction/ConstructionPart.h"
#include "UI/Construction/UnicStructure.h"

UPanelWidget* UStructureMenuWidget::GetStructurePanel()
{
	return structureBlueprintsPanel;
}

//First step to create slots of unlocked recipes for select to build, continue to CreateStructureSlot
void UStructureMenuWidget::SlotCreation(UDataTable* table, TArray<FName> names, EStructureType type)
{
	if (!IsValid(table)) return;
	dataTable = table;

	for (FName n : names)
	{
		if (!table->GetRowNames().Contains(n)) return;

		FConstructionData* data = dataTable->FindRow<FConstructionData>(n, TEXT("ProcessingRows"));
		if (!data) return;

		switch (type)
		{
			//All recipes
			case EStructureType::None:
				CreateStructureSlot(n);
				break;
			//Only filtered recipes
			default:
				if (data->type == type)
				{
					CreateStructureSlot(n);
				}
		}
	}
}

//Create the slots individually
void UStructureMenuWidget::CreateStructureSlot(FName row)
{
	FConstructionData* dataRow = dataTable->FindRow<FConstructionData>(row,"");

	UUnicStructure* newStructureBlueprint = CreateWidget<UUnicStructure>(GetWorld(), structuresWidget); 
	newStructureBlueprint->SetData(*dataRow);
	Cast<UUniformGridPanel>(structureBlueprintsPanel)->AddChildToUniformGrid(newStructureBlueprint, 0, structureBlueprintsPanel->GetAllChildren().Num());

}

void UStructureMenuWidget::RemoveSlots()
{
	Cast<UUniformGridPanel>(structureBlueprintsPanel)->ClearChildren();
}

USelectionDataWidget* UStructureMenuWidget::GetSelectionWidget()
{
	return selectionWidget;
}


