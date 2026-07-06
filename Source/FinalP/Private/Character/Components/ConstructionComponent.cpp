// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/ConstructionComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Character/PlayerCharacter.h"
#include "Components/PanelWidget.h"
//#include "Structs/ConstructionData.h"
#include "Character/Components/InventoryComponent.h"
#include "Construction/ConstructionPart.h"
#include "UI/PlayerHUD.h"
#include "UI/Construction/StructureMenuWidget.h"
#include "UI/Construction/UnicStructure.h"
#include "Engine/GameInstance.h"
#include "World/TechTreeSubsystem.h"


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

	if (UGameInstance* gameInstance = GetWorld()->GetGameInstance()){
		if (UTechTreeSubsystem* techTree = gameInstance->GetSubsystem<UTechTreeSubsystem>()){
			techTree->OnTechNodeUnlock.AddDynamic(this, &UConstructionComponent::HandleTechNodeUnlocked);
		}
	}

	// ...
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

	UTechTreeSubsystem* techTree = nullptr;
	if (UGameInstance* gameInstance = GetWorld()->GetGameInstance()){
		techTree = gameInstance->GetSubsystem<UTechTreeSubsystem>();
	}

	TArray<FName> rowNames = dataTable->GetRowNames();

	for (FName name : rowNames)
	{
		if (dataRow!=nullptr) dataRow=nullptr;

		if (!unlockedStructuresData.Contains(name))
		{
			GetCraftablesStructures(name);
			if (dataRow->unlockLevel<=actLevel || (techTree && techTree->IsBuildingUnlocked(name)))
			{
				unlockedStructuresData.Add(name);
			}
		}

		if (dataRow!=nullptr) dataRow=nullptr;
	}
}

void UConstructionComponent::HandleTechNodeUnlocked(FName nodeId)
{
	LoadRecipes();
	UpdateRecipeSlots();
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
	hud->GetStructureWidget()->RemoveSlots();
	hud->GetStructureWidget()->SlotCreation(dataTable, unlockedStructuresData, defaultType);
}

void UConstructionComponent::UpdateFilterRecipeSlots(EStructureType type)
{
	APlayerHUD* hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->GetStructureWidget()->RemoveSlots();
	hud->GetStructureWidget()->SlotCreation(dataTable, unlockedStructuresData, type);
}

//Debug
void UConstructionComponent::PrintUnlockedRecipes()
{
	for (FName name : unlockedStructuresData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, name.ToString());

	}
}

void UConstructionComponent::CreateStructure(TSubclassOf<AConstructionPart>& constructionPart, TMap<int, int> cost)
{
	if (!IsValid(constructionPart)) return;

	if (actSelectedPart==nullptr || actSelectedPart!= constructionPart)
	{
		actSelectedPart = constructionPart;
	}

	actStructureCost = cost;
	
	CreateGhost();
	
	//actBuilding->SetTransparentMaterial();
}

void UConstructionComponent::CreateGhost()
{
	previewMesh = NewObject<UStaticMeshComponent>(player, UStaticMeshComponent::StaticClass(), "PreviewMesh");

	if (!IsValid(previewMesh)) return;

	previewMesh->RegisterComponent();
	previewMesh->AttachToComponent(player->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	previewMesh->CreationMethod = EComponentCreationMethod::Instance;
	previewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	actBuilding = GetWorld()->SpawnActor<AConstructionPart>(actSelectedPart, {0.f,0.f,0.f}, structureRotator);
	actBuilding->SetActorHiddenInGame(true);
	
	previewMesh->SetStaticMesh(actBuilding->GetStructureMesh()->GetStaticMesh());
}

void UConstructionComponent::MoveStructure()
{
	if (!IsValid(actBuilding)) return;
	
	FVector startLocation;
	FRotator rotation;

	if (!IsValid(player)) return;
	player->GetActorEyesViewPoint(startLocation, rotation);

	FVector endLocation = startLocation + (rotation.Vector() * 400.f);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);
	params.AddIgnoredActor(actBuilding);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params);
	
	switch (bHit)
	{
		case true:
			structurePlace = {round(hitResult.Location.X/100.f)*100.f,round(hitResult.Location.Y/100.f)*100.f,round(hitResult.Location.Z/100.f)*100.f};
			previewMesh->SetWorldTransform(FTransform(structureRotator,structurePlace,{1.f,1.f,1.f}));
			AvailableColor(CheckOverlap());
			break;
		case false:
			structurePlace = {round(endLocation.X/100)*100,round(endLocation.Y/100)*100,round(endLocation.Z/100)*100};
			previewMesh->SetWorldTransform(FTransform(structureRotator,structurePlace,{1.f,1.f,1.f}));
			AvailableColor(CheckOverlap());
			break;
	}
}

bool UConstructionComponent::CheckOverlap()
{
	UWorld* World = GetWorld();
	if (!World || !previewMesh)
	{
		return false;
	}

	FVector Location = previewMesh->Bounds.Origin;;
	FQuat Rotation = previewMesh->GetComponentQuat();

	FVector origin;
	FVector extent;

	previewMesh->GetLocalBounds(origin, extent);

	extent *= 0.25f;

	FHitResult Hit;

	FCollisionQueryParams params;
	params.AddIgnoredActor(player);

	bool bHit = !World->SweepSingleByChannel(Hit, Location, Location, Rotation, ECC_WorldStatic, FCollisionShape::MakeBox(extent), params);

	DrawDebugBox(
	GetWorld(),
	Location,
	extent,
	Rotation,
	bHit ? FColor::Red : FColor::Green,
	false,
	0.0f,
	0,
	2.0f
);
	return bHit;
}

void UConstructionComponent::AvailableColor(bool canBuild)
{
	buildable = canBuild;
	switch (canBuild)
	{
		case true:
			previewMesh->SetMaterial(0, transparentMaterial);
			break;
		case false:
			previewMesh->SetMaterial(0, transparentBadMaterial);
			break;
	}
}

void UConstructionComponent::RotateLeftStructure()
{
	
	if (!IsValid(actBuilding)) return;

	actBuilding->SetActorRotation(actBuilding->GetActorRotation()+FRotator(0, 15, 0));
	structureRotator = actBuilding->GetActorRotation();
}

void UConstructionComponent::RotateRightStructure()
{
	if (!IsValid(actBuilding)) return;

	actBuilding->SetActorRotation(actBuilding->GetActorRotation()+FRotator(0, -15, 0));
	structureRotator = actBuilding->GetActorRotation();
}

void UConstructionComponent::PlaceStructure()
{
	if (!IsValid(actBuilding)) return;
	//if (!actBuilding->GetValidConstruct()) return;
	if (!buildable) return;

	bool canContinue = true;
	for (const TPair<int, int>& pair : actStructureCost)
	{
		if (canContinue == false) continue;
		canContinue = player->GetInventory()->RemoveItem(pair.Key, pair.Value);
	}
	
	previewMesh->DestroyComponent();

	actBuilding->SetActorRotation(structureRotator);
	actBuilding->SetActorLocation(structurePlace);
	actBuilding->SetActorHiddenInGame(false);
	actBuilding->SetActorEnableCollision(true);
	actBuilding->ChangeColl();
	actBuilding->OnPlaced();

	actBuilding = nullptr;
	
	CreateGhost();
	
	if (!canContinue)
	{
		player->NoMoreMaterial();
	}
}

void UConstructionComponent::EndBuild()
{
	if (actBuilding != nullptr)
	{
		actBuilding->Destroy();
		previewMesh->DestroyComponent();
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

