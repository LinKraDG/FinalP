// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/StructureMenuWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/ConstructionComponent.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Construction/ConstructionData.h"
#include "Construction/ConstructionPart.h"
#include "UI/Construction/UnicStructure.h"

UPanelWidget* UStructureMenuWidget::GetStructurePanel()
{
	return structureBlueprintsPanel;
}

//First step to create slots of unlocked recipes for select to build, continue to CreateStructureSlot
void UStructureMenuWidget::SlotCreation(UDataTable* table, TArray<FName> names)
{
	if (!IsValid(table)) return;
	dataTable = table;
	
	for (FName n : names)
	{
		if (!table->GetRowNames().Contains(n)) return;

		CreateStructureSlot(n);
	}
}

//Create the slots individually
void UStructureMenuWidget::CreateStructureSlot(FName row)
{
	FConstructionData* dataRow = dataTable->FindRow<FConstructionData>(row,"");

	UUnicStructure* newStructureBlueprint = CreateWidget<UUnicStructure>(GetWorld(), structuresWidget); 
	newStructureBlueprint->SetData(*dataRow);
	
	//TODO-- añadir paneles para cada categoria y cada tipo de estructura

	//TODO-- una vez añadidos paneles para cada uno hacer que se añadan como hijos de dichos paneles
	/*switch (dataRow->type)
	{
		case EStructureType::Decorative:
			break;
		case EStructureType::Production:
			break;
		case EStructureType::Logistic:
			break;
		case EStructureType::Organization:
			break;
		default:
			break;
	}*/

	Cast<UUniformGridPanel>(structureBlueprintsPanel)->AddChildToUniformGrid(newStructureBlueprint, 0, structureBlueprintsPanel->GetAllChildren().Num());


	
}

USelectionDataWidget* UStructureMenuWidget::GetSelectionWidget()
{
	return selectionWidget;
}


