// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/UnicStructure.h"
//#include "Components/Border.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/PlayerHUD.h"
#include "UI/Construction/SelectionDataWidget.h"
#include "UI/Construction/StructureMenuWidget.h"

void UUnicStructure::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	selectionButton->OnPressed.AddUniqueDynamic(this, &UUnicStructure::OnStructureButtonPressed);
}

//Press button for see all information of the structure at SelectionDataWidget
void UUnicStructure::OnStructureButtonPressed()
{
	hud = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->GetStructureWidget()->GetSelectionWidget()->SelectStructure(constructionData);
}

UBorder* UUnicStructure::GetFrame()
{
	return frame;
}

//Creating slot: setting data to the slot and set name and images
void UUnicStructure::SetData(FConstructionData data)
{
	constructionData = data;
	ID = data.structure_ID;
	imageStructureFrame = data.image;
	
	nameText->SetText(FText::FromString(data.name));
	frame->SetBrushFromTexture(imageStructureFrame);
}

