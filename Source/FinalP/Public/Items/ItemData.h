#pragma once

#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Wood,
	Stone,
	Metal
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	EItemType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UTexture2D* image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	AActor* item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	AActor* itemClass;


	
	
};
