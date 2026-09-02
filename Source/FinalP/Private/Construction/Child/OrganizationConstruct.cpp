// Fill out your copyright notice in the Description page of Project Settings.


#include "Construction/Child/OrganizationConstruct.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/InventoryComponent.h"


// Sets default values
AOrganizationConstruct::AOrganizationConstruct()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
}

// Called when the game starts or when spawned
void AOrganizationConstruct::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOrganizationConstruct::Interact_Implementation(AActor* playerCharacter)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(playerCharacter);

	if (!IsValid(player) || !IsValid(player->inventoryComponent)) return;

	
}

// Called every frame
/*void AOrganizationConstruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

