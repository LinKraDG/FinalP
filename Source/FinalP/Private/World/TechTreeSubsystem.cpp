// Fill out your copyright notice in the Description page of Project Settings.


#include "World/TechTreeSubsystem.h"

#include "World/DatabaseSettings.h"
#include "World/DatabaseSubsystem.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"
#include "Construction/ConstructionPart.h"

void UTechTreeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UDatabaseSettings* settings = GetDefault<UDatabaseSettings>()){
		techTable = settings->techTable.LoadSynchronous();
	}

	UnlockDefaultNodes();
	RebuildSets();
}

void UTechTreeSubsystem::Deinitialize()
{
	techTable = nullptr;
	unlockedNodes.Reset();
	unlockedRecipes.Reset();
	unlockedBuildings.Reset();
	Super::Deinitialize();
}


bool UTechTreeSubsystem::GetTechNode(FName nodeId, FTechNode& outNode) const
{
	if (!techTable || nodeId.IsNone()){
		return false;
	}

	if (const FTechNode* row = techTable->FindRow<FTechNode>(nodeId, TEXT("GetTechNode"), false)){
		outNode = *row;
		return true;
	}
	return false;
}

bool UTechTreeSubsystem::IsNodeUnlocked(FName nodeId) const
{
	return unlockedNodes.Contains(nodeId);
}

bool UTechTreeSubsystem::CanResearch(FName nodeId) const
{
	if (IsNodeUnlocked(nodeId)){
		return false;
	}

	FTechNode node;
	if (!GetTechNode(nodeId, node)){
		return false;
	}

	for (const FName& prereq : node.preRequisites){
		if (!IsNodeUnlocked(prereq)){
			return false;
		}
	}
	return HasRequiredConstructions(node);
}

void UTechTreeSubsystem::GetNoPrerequisites(FName nodeId, TArray<FName>& outMissing) const
{
	outMissing.Reset();

	FTechNode node;
	if (!GetTechNode(nodeId, node)){return;}

	for (const FName& prereq : node.preRequisites){
		if (!IsNodeUnlocked(prereq)){
			outMissing.Add(prereq);
		}
	}
}

void UTechTreeSubsystem::GetNodeCost(FName nodeId, TArray<FItemAmount>& outCost) const
{
	outCost.Reset();

	FTechNode node;
	if (GetTechNode(nodeId, node)){
		outCost = node.cost;
	}
}

void UTechTreeSubsystem::GetAvailableNodes(TArray<FName>& outNodeIds) const
{
	outNodeIds.Reset();
	if (!techTable){return;}

	for (const TPair<FName, uint8*>& pair : techTable->GetRowMap()){
		if (CanResearch(pair.Key)){
			outNodeIds.Add(pair.Key);
		}
	}
}

void UTechTreeSubsystem::GetAllNodes(TArray<FName>& outNodeIds) const
{
	outNodeIds.Reset();
	if (techTable){
		outNodeIds = techTable->GetRowNames();
	}
}

bool UTechTreeSubsystem::Research(FName nodeId)
{
	if (!CanResearch(nodeId)){
		return false;
	}

	FTechNode node;
	if (!GetTechNode(nodeId, node)){
		return false;
	}

	unlockedNodes.Add(nodeId);
	ApplyNodeUnlocks(node);

	OnTechNodeUnlock.Broadcast(nodeId);
	return true;
}

void UTechTreeSubsystem::TryAutoResearchAll()
{
	if (!techTable){return;}

	TArray<FName> allNodes = techTable->GetRowNames();

	bool bAnyResearched = true;
	while (bAnyResearched){
		bAnyResearched = false;
		for (const FName& nodeId : allNodes){
			if (!IsNodeUnlocked(nodeId) && CanResearch(nodeId)){
				if (Research(nodeId)){
					bAnyResearched = true;
				}
			}
		}
	}
}

bool UTechTreeSubsystem::IsRecipeUnlocked(FName recipeId) const
{
	return unlockedRecipes.Contains(recipeId);
}

bool UTechTreeSubsystem::IsBuildingUnlocked(FName buildingId) const
{
	return unlockedBuildings.Contains(buildingId);
}

void UTechTreeSubsystem::GetUnlockedRecipes(TArray<FName>& outRecipeIds) const
{
	outRecipeIds.Reset();

	UDatabaseSubsystem* db = GetGameInstance() ? GetGameInstance()->GetSubsystem<UDatabaseSubsystem>() : nullptr;
	if (!db){return;}

	TArray<FName> candidates;
	db->GetRecipesByUnlockLevel(MAX_int32, candidates);

	for (const FName& recipeId : candidates){
		if (IsRecipeUnlocked(recipeId)){
			outRecipeIds.Add(recipeId);
		}
	}
}

bool UTechTreeSubsystem::HasRequiredConstructions(const FTechNode& node) const
{
	if (node.requiredConstructions.Num() == 0){
		return true;
	}

	UWorld* world = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr;
	if (!world){
		return false;
	}

	for (const TSubclassOf<AConstructionPart>& requiredClass : node.requiredConstructions){
		if (!requiredClass){continue;}

		bool bFound = false;
		for (TActorIterator<AConstructionPart> it(world); it; ++it){
			AConstructionPart* part = *it;
			if (IsValid(part) && part->IsPlaced() && part->IsA(requiredClass)){
				bFound = true;
				break;
			}
		}

		if (!bFound){
			return false;
		}
	}
	return true;
}

void UTechTreeSubsystem::ApplyNodeUnlocks(const FTechNode& node)
{
	for (const FName& recipeId : node.unlockedRecipes){
		unlockedRecipes.Add(recipeId);
	}
	for (const FName& buildingId : node.unlockedBuildings){
		unlockedBuildings.Add(buildingId);
	}
}

void UTechTreeSubsystem::RebuildSets()
{
	unlockedRecipes.Reset();
	unlockedBuildings.Reset();

	for (const FName& nodeId : unlockedNodes){
		FTechNode node;
		if (GetTechNode(nodeId, node)){
			ApplyNodeUnlocks(node);
		}
	}
}

void UTechTreeSubsystem::UnlockDefaultNodes()
{
	if (!techTable){return;}

	for (const TPair<FName, uint8*>& pair : techTable->GetRowMap()){
		const FTechNode* node = reinterpret_cast<const FTechNode*>(pair.Value);
		if (node && node->bUnlockedByDefault){
			unlockedNodes.Add(pair.Key);
		}
	}
}
