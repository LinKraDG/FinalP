// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/BlueprintIngredientWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UBlueprintIngredientWidget::SetData(FItemData ingredient, int amount)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter());
	inventory = player->GetInventory();

	ingredientsData = ingredient;
	
	itemID = ingredientsData.item_ID;
	imageFrame->SetBrushFromTexture(ingredientsData.image);
	
	nameText->SetText(FText::FromString(ingredientsData.name));
	costText->SetText(FText::AsNumber(amount));
	bpIngredient.amount = amount;

	Refresh();

	if (inventory)
	{
		inventory->OnItemAmountChanged.AddUObject(this, &UBlueprintIngredientWidget::OnInventoryItemChanged);
	}
	
}

bool UBlueprintIngredientWidget::GetHasMaterials()
{
	return hasMaterials;
}

int UBlueprintIngredientWidget::GetItemID()
{
	return ingredientsData.item_ID;
}

int UBlueprintIngredientWidget::GetCost()
{
	return bpIngredient.amount;
}

void UBlueprintIngredientWidget::Refresh()
{
	
	if (!inventory)
	{
		return;
	}
	
	int amount = inventory->GetItemAmount(itemID);

	actAmountText->SetText(FText::AsNumber(amount));

	if (amount >= bpIngredient.amount)
	{
		actAmountText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		hasMaterials = true;
	}
	else
	{
		actAmountText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		hasMaterials = false;
	}
}

void UBlueprintIngredientWidget::OnInventoryItemChanged(FInventoryItem Item)
{
	Refresh();
}

void UBlueprintIngredientWidget::NativeDestruct()
{
	/*APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());;
	UInventoryComponent* inventory = player->GetInventory();
	
	if (inventory)
	{
		inventory->OnItemAmountChanged.RemoveAll(this);
	}
	
	Super::NativeDestruct();*/
}

