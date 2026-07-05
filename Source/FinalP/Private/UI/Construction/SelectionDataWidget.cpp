// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/SelectionDataWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/ConstructionComponent.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Construction/ConstructionPart.h"
#include "UI/Construction/BlueprintIngredientWidget.h"

void USelectionDataWidget::Refresh()
{
	if (IsValid(selectedStructure))
	{
		CheckHasMaterials();
	}
}

void USelectionDataWidget::OnInventoryItemChanged(FInventoryItem Item)
{
	Refresh();
}

void USelectionDataWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter());
	inventory = player->GetInventory();
	
	inventory->OnItemAmountChanged.AddUObject(this, &USelectionDataWidget::OnInventoryItemChanged);
	buildButton->OnPressed.AddUniqueDynamic(this, &USelectionDataWidget::OnBuildButtonPressed);
}

//Press button for close crafting menu and enter to build mode
void USelectionDataWidget::OnBuildButtonPressed()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(player)) return;

	TMap<int/*itemID*/, int/*amount*/> cost;
	
	for (auto Element : structureCostPanel->GetAllChildren())
	{
		UBlueprintIngredientWidget* elem = Cast<UBlueprintIngredientWidget>(Element);
		cost.Add(elem->GetItemID(), elem->GetCost());
	}
	
	player->SetConstructionMode(selectedStructure, cost);
}

//Load data to this widget
void USelectionDataWidget::SelectStructure(FConstructionData structureData)
{

	selectedStructure = structureData.structure;

	structureNameText->SetText(FText::FromString(structureData.name));
	structureBorder->SetBrushFromTexture(structureData.image);
	descriptionText->SetText(FText::FromString(structureData.desciption));

	
	RemoveSlots();
	//Foreach para cada material necesario y crear lineas para cada material
	for (FCraftIngredient itemIngredient : structureData.craftIngredients)
	{
		CreateIngredientSlot(itemIngredient);
	}
	
	CheckHasMaterials();
	//TODO-- Hacer que el boton de construir se ilumine cuando se posean todos los ingredientes
}

void USelectionDataWidget::CreateIngredientSlot(FCraftIngredient ingredient)
{
	FItemData* dataRow = ingredient.item.DataTable->FindRow<FItemData>(ingredient.item.RowName,"");
	UBlueprintIngredientWidget* newStructureCost = CreateWidget<UBlueprintIngredientWidget>(GetWorld(), costWidget);
	
	newStructureCost->SetData(*dataRow, ingredient.amount);
	UVerticalBox* Grid = Cast<UVerticalBox>(structureCostPanel);

	Grid->AddChildToVerticalBox(newStructureCost);
}

void USelectionDataWidget::RemoveSlots()
{
	Cast<UVerticalBox>(structureCostPanel)->ClearChildren();
}

bool USelectionDataWidget::CheckHasMaterials()
{
	for (auto Element : structureCostPanel->GetAllChildren())
	{
		if (!Cast<UBlueprintIngredientWidget>(Element)->GetHasMaterials())
		{
			buildButton->SetVisibility(ESlateVisibility::Hidden);
			return false;
		}
	}
	buildButton->SetVisibility(ESlateVisibility::Visible);
	return true;
}
