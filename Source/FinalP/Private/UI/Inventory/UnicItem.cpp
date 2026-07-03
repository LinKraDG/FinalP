// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/UnicItem.h"

//#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Structs/ItemData.h"
#include "UI/PlayerHUD.h"

void UUnicItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	itemButton->OnPressed.AddUniqueDynamic(this, &UUnicItem::OnItemButtonPressed);
	dropButton->OnPressed.AddUniqueDynamic(this, &UUnicItem::OnDropButtonPressed);
}

UBorder* UUnicItem::GetFrame()
{
	return frame;
}

UBorder* UUnicItem::GetItem()
{
	return item;
}

UTextBlock* UUnicItem::GetQuantityText()
{
	return quantity;
}

UVerticalBox* UUnicItem::GetOptions()
{
	return options;
}

void UUnicItem::SetNoneImage()
{
	frame->SetVisibility(ESlateVisibility::Visible);
	frame->SetBrushFromTexture(imageFrame);
}

void UUnicItem::SetFrameImage()
{
	frame->SetVisibility(ESlateVisibility::Visible);
	frame->SetBrushFromTexture(imageItemFrame);
}

void UUnicItem::OnItemButtonPressed()
{
	UTexture2D* image = Cast<UTexture2D>(frame->Background.GetResourceObject());
	if (!IsValid(image)) return;

	if (image == imageItemFrame)
	{
		hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		hud->GetInventoryWidget()->ShowHideOptions(ID);
	}
}

void UUnicItem::OnDropButtonPressed()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(player)) return;

	UInventoryComponent* inventory = player->GetInventory();
	if (!IsValid(inventory)) return;

	inventory->UnloadItem(inventory->GetItem(ID), ID);

	hud->GetInventoryWidget()->ShowHideOptions(ID);

	inventory->PrintInventory();

	hud = nullptr;
}

/*void UUnicItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                     UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}*/
