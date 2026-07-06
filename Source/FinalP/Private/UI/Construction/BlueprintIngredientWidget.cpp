// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/BlueprintIngredientWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UBlueprintIngredientWidget::SetData(FCraftIngredient  ingredient)
{
	UInventoryComponent* inventory = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())->GetInventory(); 

	FItemData* data = ingredient.item.GetRow<FItemData>("");
	
	ingredientsData = *data;
	
	itemID = data->item_ID;
	imageFrame->SetBrushFromTexture(data->image);
	
	nameText->SetText(FText::FromString(data->name));
	//actAmountText->SetText(FText::FromInt(data->name));
	costText->SetText(FText::AsNumber(ingredient.amount));
}
