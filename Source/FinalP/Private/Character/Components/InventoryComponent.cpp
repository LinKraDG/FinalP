// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/InventoryComponent.h"

#include "Character/PlayerCharacter.h"
#include "Components/Border.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBox.h"
#include "Items/ItemData.h"
#include "UI/PlayerHUD.h"
#include "UI/UnicItem.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

int UInventoryComponent::GetSize()
{
	return actSize;
}

FHollowInfo UInventoryComponent::CheckHollow()
{
	
	int voidElement = -2;
	int index = 0;

	for (FItemData i  : inventoryData)
	{
		if (i.image == nullptr)
		{
			voidElement = index;
			break;
		}

		index++;
	}
	
	if (voidElement == -2)
	{
		return FHollowInfo(false, voidElement);
	}
	else
	{
		return FHollowInfo(true, voidElement);
	}
}

void UInventoryComponent::LoadItem(FItemData item, int index)
{
	if (index>=0 || index<actSize)
	{
		inventoryData[index] = item;
		APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (!IsValid(player)) return;

		player->interactiveItem->Destroy();
	}
}

void UInventoryComponent::UnloadItem(FItemData item, int index)
{
	FItemData leadedItem = item;
	inventoryData[index] = FItemData();
}

FItemData UInventoryComponent::GetItem(int index)
{
	return inventoryData[index];
}

void UInventoryComponent::PrintInventory()
{
	APlayerHUD* hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	UPanelWidget* panel = Cast<UPanelWidget>(hud->GetInventoryWidget()->GetItemsPanel());
	UUnicItem* selectedItem = nullptr;
	int index = 0;
	
	for (FItemData i : inventoryData)
	{
		if (index>actSize) break;

		FItemData selected = i;
		int inventoryCount = index;
		
		selectedItem = Cast<UUnicItem>(panel->GetChildAt(inventoryCount));
		selectedItem->SetVisibility(ESlateVisibility::Visible);
		//selectedItem->GetFrame()->SetVisibility(ESlateVisibility::Visible);

		if (selected.image == nullptr )
		{
			selectedItem->SetNoneImage();
			selectedItem->GetItem()->SetVisibility(ESlateVisibility::Hidden);
			selectedItem->GetOptions()->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			selectedItem->SetFrameImage();
			selectedItem->GetItem()->SetBrushFromTexture(i.image);
			selectedItem->GetItem()->SetVisibility(ESlateVisibility::Visible);
			selectedItem->GetOptions()->SetVisibility(ESlateVisibility::Hidden);
		}
		
		index++;
	}
}


// Called every frame
/*void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

