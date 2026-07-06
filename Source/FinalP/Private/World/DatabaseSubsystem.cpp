// Fill out your copyright notice in the Description page of Project Settings.


#include "World/DatabaseSubsystem.h"

#include "World/DatabaseSettings.h"
#include "Engine/DataTable.h"

void UDatabaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UDatabaseSettings* settings = GetDefault<UDatabaseSettings>()){
		itemTable = settings->itemTable.LoadSynchronous();
		recipeTable = settings->recipeTable.LoadSynchronous();
	}
}

void UDatabaseSubsystem::Deinitialize()
{
	itemTable = nullptr;
	recipeTable = nullptr;
	Super::Deinitialize();
}

bool UDatabaseSubsystem::GetItem(FName itemId, FItemData& outItem) const
{
	if (!itemTable || itemId.IsNone()){
		return false;
	}

	if (const FItemData* row = itemTable->FindRow<FItemData>(itemId, TEXT("GetItem"), false)){
		outItem = *row;
		return true;
	}
	return false;
}

bool UDatabaseSubsystem::IsValidItem(FName itemId) const
{
	FItemData itemData;
	return GetItem(itemId, itemData);
}

FItemData UDatabaseSubsystem::ItemStack(FName itemId, int32 quantity) const
{
	FItemData stack;
	GetItem(itemId, stack);
	return stack;
}

void UDatabaseSubsystem::GetAllItemIds(TArray<FName>& outIds) const
{
	outIds.Reset();
	if (itemTable){
		outIds = itemTable->GetRowNames();
	}
}

bool UDatabaseSubsystem::GetRecipe(FName recipeId, FRecipeData& outRecipe) const
{
	if (!recipeTable || recipeId.IsNone()){
		return false;
	}

	if (const FRecipeData* row = recipeTable->FindRow<FRecipeData>(recipeId, TEXT("GetRecipe"), false)){
		outRecipe = *row;
		return true;
	}
	return false;
}

void UDatabaseSubsystem::GetAllRecipeIds(TArray<FName>& outIds) const
{
	outIds.Reset();
	if (recipeTable){
		outIds = recipeTable->GetRowNames();
	}
}

void UDatabaseSubsystem::GetRecipesByUnlockLevel(int32 maxUnlockLevel, TArray<FName>& outRecipeIds) const
{
	outRecipeIds.Reset();
	if (!recipeTable){return;}

	for (const TPair<FName, uint8*>& pair : recipeTable->GetRowMap()){
		const FRecipeData* recipe = reinterpret_cast<const FRecipeData*>(pair.Value);
		if (recipe && recipe->unlockLevel <= maxUnlockLevel)
		{
			outRecipeIds.Add(pair.Key);
		}
	}
}
