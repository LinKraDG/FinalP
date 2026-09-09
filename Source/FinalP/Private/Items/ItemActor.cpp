// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemActor.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AItemActor::AItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("staticMesh");
	staticMesh->SetupAttachment(RootComponent);

	//collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CajaColision"));
	//collision->SetupAttachment(staticMesh);
	//collision->SetGenerateOverlapEvents(true);
	//collision->SetCollisionResponseToAllChannels(ECR_Overlap);

	
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::DestroyResource()
{
	///////////////
	this->Destroy();
}

void AItemActor::Interact_Implementation(AActor* playerCharacter)
{
	player = Cast<APlayerCharacter>(playerCharacter);
	if (!IsValid(player) || !IsValid(player->inventoryComponent))
	{
		return;
	}
	
	player->SetInteractItem(this);
	
	player->AnimationSelector();
}

/*void AItemActor::ReduceQuantity(int extract)
{
	quantity -= extract;
	Interact_Implementation(player);
}*/

EItemType AItemActor::GetTypeResource()
{
	FItemData* dataRow = itemData.DataTable->FindRow<FItemData>(itemData.RowName,"");
	return dataRow->type;
}

void AItemActor::GiveResource()
{
	FItemData* dataRow = itemData.DataTable->FindRow<FItemData>(itemData.RowName,"");

	player->inventoryComponent->LoadItem(dataRow->item_ID, FMath::RandRange(minQuantity, maxQuantity));
	if (FMath::FRand() <= 0.25f) DestroyResource();
}

// Called every frame
/*void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

