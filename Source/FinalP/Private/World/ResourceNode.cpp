// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ResourceNode.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "World/DatabaseSubsystem.h"

AResourceNode::AResourceNode()
{
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);
}

bool AResourceNode::PutMachine()
{
	if (bOccupied){
		return false;
	}
	bOccupied = true;
	return true;
}

void AResourceNode::ReleaseMachine()
{
	bOccupied = false;
}

void AResourceNode::Interact_Implementation(AActor* actor)
{
	APlayerCharacter* playerChar = Cast<APlayerCharacter>(actor);
	if (!IsValid(playerChar) || !IsValid(playerChar->inventoryComponent)){return;}

	if (resourceItemId.IsNone()){return;}

	if (IsEmpty()){return;}

	const int32 extracted = ExtractUnits(getPerInteract);
	if (extracted <= 0){return;}

	FItemData stack{};
	bool bResolved = false;
	if (UGameInstance* gi = GetGameInstance()){
		if (UDatabaseSubsystem* db = gi->GetSubsystem<UDatabaseSubsystem>()){
			bResolved = db->GetItem(resourceItemId, stack);
		}
	}

	if (!bResolved){
		if (!bInfinite){
			amountRemaining += extracted;
		}
		return;
	}

	playerChar->inventoryComponent->LoadItem(stack, extracted);
}

int32 AResourceNode::ExtractUnits(int32 requested)
{
	if (requested <= 0){
		return 0;
	}

	if (bInfinite){
		return requested;
	}

	const int32 taken = FMath::Min(requested, amountRemaining);
	amountRemaining -= taken;
	return taken;
}
