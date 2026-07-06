// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Construction/Machine/MachineBase.h"
#include "StorageMachine.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FINALP_API AStorageMachine : public AMachineBase
{
	GENERATED_BODY()

public:
	AStorageMachine();

	virtual bool CanProcess_Implementation() override;

	virtual void Process_Implementation() override;

	UFUNCTION(BlueprintCallable)
	bool DepositItem(FItemData item);

	UFUNCTION(BlueprintCallable)
	bool WithdrawItem(int32 index, FItemData& outItem);

	UFUNCTION(BlueprintPure)
	const TArray<FItemData>& GetStoredItems() const { return outputInventory; }

	UFUNCTION(BlueprintPure)
	int32 GetStoredCount() const;

	UFUNCTION(BlueprintPure)
	bool IsFull() const { return outputInventory.Num() >= outputSpace; }
};
