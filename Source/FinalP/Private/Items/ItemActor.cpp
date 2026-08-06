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

	/*FVector BoxMin;
	FVector BoxMax;
	staticMesh->GetLocalBounds(BoxMin, BoxMax);

	// 4. Calcular el tamaño al doble y el centro
	FVector Center = (BoxMin + BoxMax) / 2.0f;
	FVector Extents = (BoxMax - BoxMin) / 2.0f; // Tamaño del radio original
	FVector DoubleExtents = Extents * 2.0f;     // Tamaño al doble

	// 5. Aplicar la posición local y el tamaño a la colisión
	collision->SetRelativeLocation(Center);
	collision->SetBoxExtent(DoubleExtents);
	
	collision->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnBeginOverlap);
	collision->OnComponentEndOverlap.AddDynamic(this, &AItemActor::OnEndOverlap);*/
}
//////////////////////////
/*void AItemActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* playerOverlap = Cast<APlayerCharacter>(OtherActor);
	if (!IsValid(playerOverlap)) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡Ha entrado!"));
}
//////////////////////////
void AItemActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	APlayerCharacter* playerOverlap = Cast<APlayerCharacter>(OtherActor);
	if (!IsValid(playerOverlap)) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡Ha salido!"));
}*/

void AItemActor::Interact_Implementation(AActor* actor)
{
	player = Cast<APlayerCharacter>(actor);
	if (!IsValid(player) || !IsValid(player->inventoryComponent))
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("¡Interactuando!"));

	player->interactiveItem = this;

	FItemData* dataRow = itemData.DataTable->FindRow<FItemData>(itemData.RowName,"");

	player->inventoryComponent->LoadItem(dataRow->item_ID, quantity);
}

void AItemActor::ReduceQuantity(int extract)
{
	quantity -= extract;
	Interact_Implementation(player);
}

// Called every frame
/*void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

