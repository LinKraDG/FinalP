#include "RuneSlotComponent.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

URuneSlotComponent::URuneSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool URuneSlotComponent::InsertRune(FItemData item, int32 amount)
{
	FItemData tableRuneData;
	if (!FindRuneDefinition(item.item_ID, tableRuneData)){return false;}

	if (insertedRune.ItemID != INDEX_NONE){
		if (insertedRune.ItemID != item.item_ID){return false;}
		insertedRune.Amount += amount;
		return true;
	}

	insertedRune.ItemID = item.item_ID;
	insertedRune.Amount = amount;

	energyDuration = tableRuneData.runeDuration;
	currentEnergyRemaining = energyDuration;
	bHasEnergy = true;
	OnEnergyStateChange.Broadcast(true);

	return true;
}

float URuneSlotComponent::GetRuneRemainingDuration() const
{
	if (currentEnergyRemaining <= 0.0f || energyDuration <= 0.0f) return 0.0f;
	return FMath::Clamp(currentEnergyRemaining / energyDuration, 0.0f, 1.0f);
}


bool URuneSlotComponent::FindRuneDefinition(int32 itemId, FItemData& outItem) const
{
	if (itemId == INDEX_NONE || !itemDataTable){ return false;}

	for (const auto& row : itemDataTable->GetRowMap()){
		const FItemData* CandidateRow = reinterpret_cast<FItemData*>(row.Value);
		if (CandidateRow && CandidateRow->item_ID == itemId && CandidateRow->bIsMagicRune){
			outItem = *CandidateRow;
			return true;
		}
	}
	return false;
}

bool URuneSlotComponent::UseNextRuneStack()
{
	if (insertedRune.ItemID == INDEX_NONE || insertedRune.Amount <= 0){return false;}

	FItemData tableRuneData;
	if (!FindRuneDefinition(insertedRune.ItemID, tableRuneData)){return false;}

	energyDuration = tableRuneData.runeDuration;
	currentEnergyRemaining = energyDuration;

	const bool bWasPowered = bHasEnergy;
	bHasEnergy = true;
	if (!bWasPowered){OnEnergyStateChange.Broadcast(true);}
	
	return true;
}

void URuneSlotComponent::StopEnergy()
{
	currentEnergyRemaining = 0.f;

	if (bHasEnergy){
		bHasEnergy = false;
		OnEnergyStateChange.Broadcast(false);
	}
}

void URuneSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bHasEnergy) { return; }

	currentEnergyRemaining -= DeltaTime;
	if (currentEnergyRemaining < 0.0f) {
		insertedRune.Amount -= 1;
		if (insertedRune.Amount <= 0) { insertedRune = FInventoryItem{}; }
		if (!UseNextRuneStack()) { StopEnergy(); }
	}
}

