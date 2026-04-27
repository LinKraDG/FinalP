#pragma once
//#include "StructUtils/InstancedStruct.h"
#include "Enums/StructuresClassify.h"

#include "ConstructionData.generated.h"

USTRUCT(BlueprintType)
struct FConstructionData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int structure_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString desciption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int unlockLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSoftClassPtr<class AItemActor>, int> craftIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AConstructionPart> structure{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStructureType type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "type == EStructureType::Decorative", EditConditionHides))
	EDecorativeStructureClassify decorativeClassify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditCondition = "type == EStructureType::Production", EditConditionHides))
	EProductionStructureClassify productionClassify;
};