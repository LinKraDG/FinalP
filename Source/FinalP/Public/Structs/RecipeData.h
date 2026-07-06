#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RecipeData.generated.h"

USTRUCT(BlueprintType)
struct FItemAmount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName itemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 amount = 1;
};

USTRUCT(BlueprintType)
struct FRecipeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText displayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> inputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemAmount> outputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float processTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 unlockLevel = 0;
};
