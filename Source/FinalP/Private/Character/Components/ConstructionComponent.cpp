// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ConstructionComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Character/PlayerCharacter.h"
#include "Components/PanelWidget.h"
#include "Construction/ConstructionData.h"
#include "Construction/ConstructionPart.h"
#include "UI/PlayerHUD.h"
#include "UI/Construction/StructureMenuWidget.h"
#include "UI/Construction/UnicStructure.h"


// Sets default values for this component's properties
UConstructionComponent::UConstructionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


// Called when the game starts
void UConstructionComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	
	// ...
	//TODO- mirar las estructuras
	//TODO- Al iniciar asociar los frames del UnicStructure por el ID a la estructura
	if (!IsValid(dataTable)) return;

	//Load unlocked recipes
	LoadRecipes();
	//Create slots for unlocked recipes
	UpdateRecipeSlots();

	//Debug
	PrintUnlockedRecipes();
}

//Load all recipes by the data table and the unlocked are included to his array
void UConstructionComponent::LoadRecipes()
{
	if (!IsValid(dataTable)) return;
	
	TArray<FName> rowNames = dataTable->GetRowNames();
	
	for (FName name : rowNames)
	{
		if (dataRow!=nullptr) dataRow=nullptr;
		
		if (!unlockedStructuresData.Contains(name))
		{
			GetCraftablesStructures(name);
			if (dataRow->unlockLevel<=actLevel)
			{
				unlockedStructuresData.Add(name);
			}
		}
		
		if (dataRow!=nullptr) dataRow=nullptr;
	}
}

//Get the row by the name
void UConstructionComponent::GetCraftablesStructures(FName rowName)
{
	if (!IsValid(dataTable)) return;
	if (!dataTable->GetRowNames().Contains(rowName)) return;
	
	dataRow = dataTable->FindRow<FConstructionData>(rowName,"");
}

//Print at HUD the widget of this crafting menu
void UConstructionComponent::PrintBPsInventory()
{
	APlayerHUD* hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	UPanelWidget* panel = Cast<UPanelWidget>(hud->GetStructureWidget()->GetStructurePanel());
	UUnicStructure* selectedStructure = nullptr;
	int index = 0;
	
	for (FConstructionData i : structureData)
	{
		//if (index>actSize) break;

		FConstructionData selected = i;
		int inventoryCount = index;
		
		selectedStructure = Cast<UUnicStructure>(panel->GetChildAt(inventoryCount));
		selectedStructure->SetVisibility(ESlateVisibility::Visible);
		//selectedItem->GetFrame()->SetVisibility(ESlateVisibility::Visible);

		if (selected.unlockLevel >= actLevel )
		{
			//selectedStructure->GetItem()->SetVisibility(ESlateVisibility::Hidden);
			//selectedStructure->GetOptions()->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			//selectedStructure->SetFrameImage();
			//selectedStructure->GetItem()->SetBrushFromTexture(i.image);
			//selectedStructure->GetQuantityText()->SetText(FText::AsNumber(i.quantity));
			//selectedStructure->GetItem()->SetVisibility(ESlateVisibility::Visible);
			//selectedStructure->GetOptions()->SetVisibility(ESlateVisibility::Hidden);
		}
		
		index++;
	}
}

//Create the slots unlocked
void UConstructionComponent::UpdateRecipeSlots()
{
	APlayerHUD* hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->GetStructureWidget()->SlotCreation(dataTable, unlockedStructuresData);
}

//Debug
void UConstructionComponent::PrintUnlockedRecipes()
{
	for (FName name : unlockedStructuresData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, name.ToString());

	}
}

void UConstructionComponent::CreateStructure(const TSubclassOf<AConstructionPart>& constructionPart)
{
	actBuilding = GetWorld()->SpawnActor<AConstructionPart>(constructionPart, FVector(0,0,0), lastRotator);
	if (!IsValid(actBuilding)) return;

	actBuilding->SetTransparentMaterial();
	actBuilding->SetActorEnableCollision(false);
}

void UConstructionComponent::MoveStructure()
{
	if (!IsValid(actBuilding)) return;
	
	FVector startLocation;
	FRotator rotation;

	if (!IsValid(player)) return;
	player->GetActorEyesViewPoint(startLocation, rotation);

	FVector endLocation = startLocation + (rotation.Vector() * 500.f);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);
	params.AddIgnoredActor(actBuilding);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params);
	FVector place;
	switch (bHit)
	{
	case true:
		place = {round(hitResult.Location.X/100)*100,round(hitResult.Location.Y/100)*100,round(hitResult.Location.Z/100)*100};
		actBuilding->SetActorLocation(place);
		break;
	case false:
		place = {round(endLocation.X/100)*100,round(endLocation.Y/100)*100,round(endLocation.Z/100)*100};
		actBuilding->SetActorLocation(place);
		break;
	}
}

void UConstructionComponent::RotateLeftStructure()
{
	
	if (!IsValid(actBuilding)) return;

	actBuilding->SetActorRotation(actBuilding->GetActorRotation()+FRotator(0, 90, 0));
	lastRotator = actBuilding->GetActorRotation();
}

void UConstructionComponent::RotateRightStructure()
{
	if (!IsValid(actBuilding)) return;

	actBuilding->SetActorRotation(actBuilding->GetActorRotation()+FRotator(0, -90, 0));
	lastRotator = actBuilding->GetActorRotation();
}

void UConstructionComponent::PlaceStructure()
{
	if (!IsValid(actBuilding)) return;

	actBuilding->ChangeMaterial();
	actBuilding->SetActorEnableCollision(true);
}

void UConstructionComponent::EndBuild()
{
	if (actBuilding != nullptr)
	{
		actBuilding->Destroy();
	}
	else
	{
		player->ChangeToDefaultMappingContext();
	}
	
	if (actBuilding != nullptr) actBuilding = nullptr;
}


// Called every frame
void UConstructionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(actBuilding)) return;
	MoveStructure();
	// ...
}

