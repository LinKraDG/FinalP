// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "World/TechNode.h"
#include "TechTreeSubsystem.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechNodeUnlock, FName, nodeId);

UCLASS()
class FINALP_API UTechTreeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintAssignable)
	FOnTechNodeUnlock OnTechNodeUnlock;

	UFUNCTION(BlueprintCallable)
	bool GetTechNode(FName nodeId, FTechNode& outNode) const;

	UFUNCTION()
	bool IsNodeUnlocked(FName nodeId) const;

	UFUNCTION(BlueprintCallable)
	bool CanResearch(FName nodeId) const;

	UFUNCTION(BlueprintCallable)
	void GetNoPrerequisites(FName nodeId, TArray<FName>& outMissing) const;

	UFUNCTION(BlueprintCallable)
	void GetNodeCost(FName nodeId, TArray<FItemAmount>& outCost) const;

	UFUNCTION(BlueprintCallable)
	void GetAvailableNodes(TArray<FName>& outNodeIds) const;

	UFUNCTION(BlueprintCallable)
	void GetAllNodes(TArray<FName>& outNodeIds) const;

	UFUNCTION(BlueprintCallable)
	bool Research(FName nodeId);

	UFUNCTION(BlueprintCallable)
	void TryAutoResearchAll();

	UFUNCTION()
	bool IsRecipeUnlocked(FName recipeId) const;

	UFUNCTION()
	bool IsBuildingUnlocked(FName buildingId) const;

	UFUNCTION(BlueprintCallable)
	void GetUnlockedRecipes(TArray<FName>& outRecipeIds) const;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> techTable{};

	TSet<FName> unlockedNodes;

	TSet<FName> unlockedRecipes;
	TSet<FName> unlockedBuildings;

	void ApplyNodeUnlocks(const FTechNode& node);

	void RebuildSets();

	bool HasRequiredConstructions(const FTechNode& node) const;

	void UnlockDefaultNodes();
};
