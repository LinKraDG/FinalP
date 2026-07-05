#pragma once

#include "CoreMinimal.h"
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int item_ID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType type = EItemType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int quantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int max_quant = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> image = nullptr;

    /*UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* item;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* itemClass;*/
};