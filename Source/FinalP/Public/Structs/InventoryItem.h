#pragma once

#include "InventoryItem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemID = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 0;
};
