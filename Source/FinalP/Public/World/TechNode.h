// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs/RecipeData.h"
#include "TechNode.generated.h"

class UTexture2D;
class AConstructionPart;

USTRUCT(BlueprintType)
struct FTechNode : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	FText displayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	FText description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	TObjectPtr<UTexture2D> icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	int32 level = 0 ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	bool bUnlockedByDefault = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	TArray<FName> preRequisites;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	TArray<TSubclassOf<AConstructionPart>> requiredConstructions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	TArray<FName> unlockedRecipes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	TArray<FName> unlockedBuildings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TechNode")
	TArray<FItemAmount> cost;
};
