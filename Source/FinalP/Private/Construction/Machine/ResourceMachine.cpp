// Fill out your copyright notice in the Description page of Project Settings.


#include "Construction/Machine/ResourceMachine.h"

#include "World/ResourceNode.h"
#include "World/DatabaseSubsystem.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"

AResourceMachine::AResourceMachine()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AResourceMachine::BeginPlay()
{
	Super::BeginPlay();
}

void AResourceMachine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (targetNode){
		targetNode->ReleaseMachine();
	}

	Super::EndPlay(EndPlayReason);
}

bool AResourceMachine::GetValidConstruct()
{
	return Super::GetValidConstruct() && FindNearestFreeNode() != nullptr;
}

void AResourceMachine::OnPlaced()
{
	Super::OnPlaced();

	targetNode = FindNearestFreeNode();
	if (targetNode){
		targetNode->PutMachine();
	}
}

AResourceNode* AResourceMachine::FindNearestFreeNode() const
{
	if (!GetWorld()){
		return nullptr;
	}

	float bestDistSq = nodeRadius * nodeRadius;
	AResourceNode* best = nullptr;

	for (TActorIterator<AResourceNode> it(GetWorld()); it; ++it){
		AResourceNode* node = *it;
		if (!IsValid(node) || node->IsOccupied()){continue;}

		const float distSq = FVector::DistSquared(node->GetActorLocation(), GetActorLocation());
		if (distSq <= bestDistSq){
			bestDistSq = distSq;
			best = node;
		}
	}
	return best;
}

bool AResourceMachine::CanProcess_Implementation()
{
	if (outputInventory.Num() >= outputSpace){
		return false;
	}

	if (!IsValid(targetNode) || targetNode->IsEmpty()){
		return false;
	}
	return true;
}

void AResourceMachine::Process_Implementation()
{
	if (!IsValid(targetNode)){return;}

	const int32 extracted = targetNode->ExtractUnits(amountPerCycle);
	if (extracted <= 0){return;}

	FItemData produced{};
	bool bResolved = false;
	if (UGameInstance* gi = GetGameInstance()){
		if (UDatabaseSubsystem* db = gi->GetSubsystem<UDatabaseSubsystem>()){
			bResolved = db->GetItem(targetNode->GetResourceItemId(), produced);
		}
	}

	if (!bResolved){return;}

	for (int32 i = 0; i < extracted; ++i){
		outputInventory.Add(produced);
	}
}
