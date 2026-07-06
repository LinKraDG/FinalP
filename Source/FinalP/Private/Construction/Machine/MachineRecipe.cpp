// Fill out your copyright notice in the Description page of Project Settings.


#include "Construction/Machine/MachineRecipe.h"

#include "World/DatabaseSubsystem.h"
#include "World/TechTreeSubsystem.h"
#include "Engine/GameInstance.h"

AMachineRecipe::AMachineRecipe()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMachineRecipe::BeginPlay()
{
	RefreshRecipe();
	Super::BeginPlay();
}

UDatabaseSubsystem* AMachineRecipe::GetItemDatabase() const
{
	const UGameInstance* gameInstance = GetGameInstance();
	return gameInstance ? gameInstance->GetSubsystem<UDatabaseSubsystem>() : nullptr;
}

UTechTreeSubsystem* AMachineRecipe::GetTechTree() const
{
	const UGameInstance* gameInstance = GetGameInstance();
	return gameInstance ? gameInstance->GetSubsystem<UTechTreeSubsystem>() : nullptr;
}

void AMachineRecipe::GetAvailableRecipes(TArray<FName>& outRecipeIds) const
{
	outRecipeIds.Reset();

	if (UTechTreeSubsystem* tech = GetTechTree()){
		tech->GetUnlockedRecipes(outRecipeIds);
	}
}

void AMachineRecipe::RefreshRecipe()
{
	bHasRecipe = false;
	currentRecipe = FRecipeData{};

	if (activeRecipeId.IsNone()){return;}

	if (UDatabaseSubsystem* db = GetItemDatabase()){
		if (db->GetRecipe(activeRecipeId, currentRecipe)){
			bHasRecipe = true;
			processTime = currentRecipe.processTime;
		}
	}
}

bool AMachineRecipe::SetRecipe(FName recipeId)
{
	if (recipeId.IsNone()){
		activeRecipeId = NAME_None;
		RefreshRecipe();
		OnMachineInventoryChange.Broadcast();
		return true;
	}

	if (UTechTreeSubsystem* tech = GetTechTree()){
		if (!tech->IsRecipeUnlocked(recipeId)){
			return false;
		}
	}

	activeRecipeId = recipeId;
	RefreshRecipe();

	OnMachineInventoryChange.Broadcast();
	TryStartProcessing();

	return bHasRecipe;
}

bool AMachineRecipe::CanProcess_Implementation()
{
	if (!bHasRecipe){
		return false;
	}

	if (outputInventory.Num() + currentRecipe.outputs.Num() > outputSpace){
		return false;
	}

	for (const FItemAmount& in : currentRecipe.inputs){
		if (CountInput(in.itemId) < in.amount){
			return false;
		}
	}
	return true;
}

void AMachineRecipe::Process_Implementation()
{
	if (!bHasRecipe){return;}

	for (const FItemAmount& in : currentRecipe.inputs){
		ConsumeInput(in.itemId, in.amount);
	}

	UDatabaseSubsystem* db = GetItemDatabase();
	for (const FItemAmount& out : currentRecipe.outputs){
		const FItemData produced = db ? db->ItemStack(out.itemId, out.amount) : FItemData{};
		for (int32 i = 0; i < out.amount; ++i){
			outputInventory.Add(produced);
		}
	}
}

int32 AMachineRecipe::CountInput(FName itemId) const
{
	const UDatabaseSubsystem* db = GetItemDatabase();
	FItemData def;
	if (!db || !db->GetItem(itemId, def)){return 0;}

	int32 total = 0;
	for (const FItemData& slot : inputInventory){
		if (slot.item_ID == def.item_ID){total += 1;}
	}
	return total;
}

void AMachineRecipe::ConsumeInput(FName itemId, int32 amount)
{
	const UDatabaseSubsystem* db = GetItemDatabase();
	FItemData def;
	if (!db || !db->GetItem(itemId, def)){return;}

	for (int32 i = inputInventory.Num() - 1; i >= 0 && amount > 0; --i){
		if (inputInventory[i].item_ID == def.item_ID){
			inputInventory.RemoveAt(i);
			--amount;
		}
	}
}
