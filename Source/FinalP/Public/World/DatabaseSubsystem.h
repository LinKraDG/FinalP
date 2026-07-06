// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/ItemData.h"
#include "Structs/RecipeData.h"
#include "DatabaseSubsystem.generated.h"

class UDataTable;

UCLASS()
class FINALP_API UDatabaseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	bool GetItem(FName itemId, FItemData& outItem) const;

	UFUNCTION()
	bool IsValidItem(FName itemId) const;

	UFUNCTION(BlueprintCallable)
	FItemData ItemStack(FName itemId, int32 quantity = 1) const;

	UFUNCTION(BlueprintCallable)
	void GetAllItemIds(TArray<FName>& outIds) const;

	UFUNCTION(BlueprintCallable)
	bool GetRecipe(FName recipeId, FRecipeData& outRecipe) const;

	UFUNCTION(BlueprintCallable)
	void GetAllRecipeIds(TArray<FName>& outIds) const;

	UFUNCTION(BlueprintCallable)
	void GetRecipesByUnlockLevel(int32 maxUnlockLevel, TArray<FName>& outRecipeIds) const;

	UDataTable* GetItemTable() const { return itemTable; }
	UDataTable* GetRecipeTable() const { return recipeTable; }

protected:
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> itemTable{};

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> recipeTable{};
};
