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

int32 UInventoryComponent::AddItem(FItemData item, int32 amount)
{
	int32 remaining = amount;
	if (remaining <= 0){
		return 0;
	}

	const int32 maxStack = item.max_quant > 0 ? item.max_quant : remaining;

	for (TPair<int, FInventoryItem>& pair : inventoryData){
		if (remaining <= 0) break;
		if (pair.Value.ItemID != item.item_ID) continue;

		const int32 space = maxStack - pair.Value.Amount;
		if (space > 0)
		{
			const int32 toAdd = FMath::Min(space, remaining);
			pair.Value.Amount += toAdd;
			remaining -= toAdd;
			NotifyChanges(pair.Value);
		}
	}

	for (TPair<int, FInventoryItem>& pair : inventoryData){
		if (remaining <= 0) break;
		if (pair.Value.ItemID != INDEX_NONE) continue;

		const int32 toAdd = FMath::Min(maxStack, remaining);
		pair.Value.ItemID = item.item_ID;
		pair.Value.Amount = toAdd;
		remaining -= toAdd;
		NotifyChanges(pair.Value);
	}
	PrintInventory();
	return remaining;
}

bool UInventoryComponent::RemoveItem(int32 itemId, int32 amount)
{
	if (amount <= 0){
		return false;
	}

	int32 totalAvailable = 0;
	for (const TPair<int, FInventoryItem>& pair : inventoryData){
		if (pair.Value.ItemID == itemId){
			totalAvailable += pair.Value.Amount;
		}
	}

	if (totalAvailable < amount){
		return false;
	}

	int32 remaining = amount;
	for (TPair<int, FInventoryItem>& pair : inventoryData){
		if (remaining <= 0) break;
		if (pair.Value.ItemID != itemId) continue;

		const int32 toRemove = FMath::Min(remaining, pair.Value.Amount);
		pair.Value.Amount -= toRemove;
		remaining -= toRemove;

		if (pair.Value.Amount <= 0){
			pair.Value = FInventoryItem();
		}

		NotifyChanges(pair.Value);
	}

	PrintInventory();
	return true;
}

void UInventoryComponent::UnloadItem(FItemData item, int index)
{
	TArray<int> keys;
	inventoryData.GenerateKeyArray(keys);

	if (!keys.IsValidIndex(index)){return;}

	FInventoryItem* IItem = inventoryData.Find(keys[index]);
	if (!IItem){return;}

	FInventoryItem removed = *IItem;
	*IItem = FInventoryItem();
	NotifyChanges(removed);
}

FItemData UInventoryComponent::GetItem(int index)
{
	TArray<FInventoryItem> vals;
	inventoryData.GenerateValueArray(vals);

	if (!vals.IsValidIndex(index) || !itemDataTable)
	{
		return FItemData{};
	}

	const FItemData* Row = itemDataTable->FindRow<FItemData>(FName(*FString::FromInt(vals[index].ItemID)), TEXT("InventoryLookup"));
	return Row ? FItemData(*Row) : FItemData{};
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

