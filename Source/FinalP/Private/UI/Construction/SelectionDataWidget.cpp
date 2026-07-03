// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/SelectionDataWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/ConstructionComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Construction/ConstructionPart.h"
#include "UI/Construction/BlueprintIngredientWidget.h"

void USelectionDataWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	buildButton->OnPressed.AddUniqueDynamic(this, &USelectionDataWidget::OnBuildButtonPressed);
}

//Press button for close crafting menu and enter to build mode
void USelectionDataWidget::OnBuildButtonPressed()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(player)) return;

	player->SetConstructionMode(selectedStructure);
}

//Load data to this widget
void USelectionDataWidget::SelectStructure(FConstructionData structureData)
{

	selectedStructure = structureData.structure;

	structureNameText->SetText(FText::FromString(structureData.name));
	structureBorder->SetBrushFromTexture(structureData.image);
	descriptionText->SetText(FText::FromString(structureData.desciption));

	buildButton->SetVisibility(ESlateVisibility::Visible);
	//Foreach para cada material necesario y crear lineas para cada material
	for (FCraftIngredient itemIngredient : structureData.craftIngredients)
	{
		CreateIngredientSlot(/*itemIngredient*/);
	}
	//TODO-- Hacer que el boton de construir se ilumine cuando se posean todos los ingredientes
}

void USelectionDataWidget::CreateIngredientSlot()
{
	UBlueprintIngredientWidget* newStructureCost = CreateWidget<UBlueprintIngredientWidget>(GetWorld(), costWidget);
	//newStructureCost->SetData()
	Cast<UUniformGridPanel>(structureCostPanel)->AddChildToUniformGrid(newStructureCost, 0, structureCostPanel->GetAllChildren().Num());

}
