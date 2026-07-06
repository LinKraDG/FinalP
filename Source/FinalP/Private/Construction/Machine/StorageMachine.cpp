#include "Construction/Machine/StorageMachine.h"

AStorageMachine::AStorageMachine()
{
	PrimaryActorTick.bCanEverTick = false;

	machineName = TEXT("Storage");
	processTime = 0.1f;
	outputSpace = 200;
}

bool AStorageMachine::CanProcess_Implementation()
{
	return inputInventory.Num() > 0 && outputInventory.Num() < outputSpace;
}

void AStorageMachine::Process_Implementation()
{
	while (inputInventory.Num() > 0 && outputInventory.Num() < outputSpace){
		outputInventory.Add(inputInventory[0]);
		inputInventory.RemoveAt(0);
	}
}

bool AStorageMachine::DepositItem(FItemData item)
{
	if (outputInventory.Num() >= outputSpace){
		return false;
	}

	outputInventory.Add(item);
	OnMachineInventoryChange.Broadcast();

	TryStartProcessing();
	return true;
}

bool AStorageMachine::WithdrawItem(int32 index, FItemData& outItem)
{
	if (!outputInventory.IsValidIndex(index)){
		return false;
	}

	outItem = outputInventory[index];
	outputInventory.RemoveAt(index);
	OnMachineInventoryChange.Broadcast();
	return true;
}

int32 AStorageMachine::GetStoredCount() const
{
	return outputInventory.Num();
}
