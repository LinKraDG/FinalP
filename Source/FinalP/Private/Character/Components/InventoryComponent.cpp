// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/InventoryComponent.h"

#include "Character/PlayerCharacter.h"
#include "Components/Border.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Items/ItemActor.h"
//#include "Structs/ItemData.h"
#include "UI/PlayerHUD.h"
#include "UI/Inventory/InventoryWidget.h"
#include "UI/Inventory/UnicItem.h"


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
	inventoryData.Reserve(actSize);
	for (int i = 0; i < actSize; i++)
	{
		inventoryData.Add(i,FInventoryItem(-1,0));
	}
	// ...
}

int UInventoryComponent::GetSize()
{
	return actSize;
}

void UInventoryComponent::LoadItem(int ID, int amount)
{
	int index = -1;
	for (const TPair<int, FInventoryItem>& pair : inventoryData)
	{
		if (pair.Value.ItemID == ID)
		{
			index = pair.Key;
			break;
		}
	}
	
	if (index != -1)
	{
		//Act exist at inventory
		FInventoryItem* IItem = inventoryData.Find(index);
		IItem->Amount += amount;

		NotifyChanges(*IItem);
	}
	else
	{
		//Not at inventory yet
		FInventoryItem NewItem;

		NewItem.ItemID = ID;
		NewItem.Amount = amount;

		for (const TPair<int, FInventoryItem>& pair : inventoryData)
		{
			if (pair.Value.ItemID == -1)
			{
				inventoryData[pair.Key] = NewItem;
				break;
			}
		}

		NotifyChanges(NewItem);
	}
}

void UInventoryComponent::UnloadItem(FItemData item, int amount)
{
	FInventoryItem* IItem = inventoryData.Find(item.item_ID);
	FInventoryItem removed = *IItem;
	removed.Amount = 0;
	inventoryData.Remove(item.item_ID);
	NotifyChanges(removed);
}

bool UInventoryComponent::RemoveItem(int itemID, int amount)
{

	if (amount <= 0) return false;

	FInventoryItem* item = nullptr;
	//FInventoryItem* Item = inventoryData.Find(itemID);

	
	for (TPair<int, FInventoryItem>& pair : inventoryData)
	{
		if (pair.Value.ItemID == itemID)
		{
			item = &pair.Value;
			break;
		}
	}

	if (!item) return false;
	
	if (item->Amount < amount) return false;

	item->Amount -= amount;

	if (item->Amount == 0)
	{
		FInventoryItem Removed = *item;

		Removed.Amount = 0;

		inventoryData.Remove(itemID);
		
		OnItemAmountChanged.Broadcast(Removed);

		return false;
	}
	else
	{
		OnItemAmountChanged.Broadcast(*item);
	}
	return true;
}

FItemData UInventoryComponent::GetItem(int index)
{
	TArray<FInventoryItem> vals;
	inventoryData.GenerateValueArray(vals);
	const FItemData* Row = itemDataTable->FindRow<FItemData>(FName(*FString::FromInt(vals[index].ItemID)),TEXT("InventoryLookup"));
	return FItemData(*Row);
}

int UInventoryComponent::GetItemAmount(int ID)
{
	for (const TPair<int, FInventoryItem>& pair : inventoryData)
	{
		if (pair.Value.ItemID == ID)
		{
			return pair.Value.Amount;
		}
	}
	return 0;
}

void UInventoryComponent::PrintInventory()
{
	APlayerHUD* hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	UPanelWidget* panel = Cast<UPanelWidget>(hud->GetInventoryWidget()->GetItemsPanel());
	UUnicItem* selectedItem = nullptr;
	int index = 0;

	for (const TPair<int, FInventoryItem>& pair : inventoryData)
	{
		if (index>actSize) break;

		const FItemData* Row = nullptr;

		for (auto& Element : itemDataTable->GetRowMap())
		{
			FItemData* actRow = (FItemData*)(Element.Value);
			if (actRow && actRow->item_ID == pair.Value.ItemID)
			{
				Row = actRow;
				break;
			}
		}
		
		int inventoryCount = index;
		
		selectedItem = Cast<UUnicItem>(panel->GetChildAt(inventoryCount));
		selectedItem->SetVisibility(ESlateVisibility::Visible);

		if (Row == nullptr )
		{
			selectedItem->SetNoneImage();
			selectedItem->GetItem()->SetVisibility(ESlateVisibility::Hidden);
			selectedItem->GetOptions()->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			selectedItem->SetFrameImage();
			selectedItem->GetItem()->SetBrushFromTexture(Row->image);
			selectedItem->GetQuantityText()->SetText(FText::AsNumber(pair.Value.Amount));
			selectedItem->GetItem()->SetVisibility(ESlateVisibility::Visible);
			selectedItem->GetOptions()->SetVisibility(ESlateVisibility::Hidden);
		}
		
		index++;
	}
}

void UInventoryComponent::NotifyChanges(FInventoryItem item)
{
	OnItemAmountChanged.Broadcast(item);
	OnInventoryUpdated.Broadcast();
}


// Called every frame
/*void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

