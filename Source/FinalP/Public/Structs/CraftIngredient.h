#pragma once
#include "ItemData.h"

#include "CraftIngredient.generated.h"

USTRUCT(BlueprintType)
struct FCraftIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int amount;
};
