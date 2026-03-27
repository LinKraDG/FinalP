// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemActor.h"

#include "IDetailTreeNode.h"
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

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::Interact_Implementation(AActor* actor)
{
	IInteractive::Interact_Implementation(actor);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡Interactuando!"));
	player = Cast<APlayerCharacter>(actor);

	player->interactiveItem = this;
	
	if (player->inventoryComponent->CheckHollow(itemData).isEmpty)
	{
		player->inventoryComponent->LoadItem(itemData, player->inventoryComponent->CheckHollow(itemData).index);
	}
		
}

void AItemActor::ReduceQuantity(int extract)
{
	itemData.quantity -= extract;
	Interact_Implementation(player);
}

// Called every frame
/*void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

