#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int item_ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int max_quant;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* image;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* itemClass;*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMagicRune = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float runeDuration;

};
