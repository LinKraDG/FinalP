// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UItemsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FName ID;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText itemName;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UStaticMesh* itemMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 itemMaxSize;
};
