// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Construction/TypeFilterWidget.h"

#include "Character/PlayerCharacter.h"
#include "Character/Components/ConstructionComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTypeFilterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	typeFilterButton->OnPressed.AddUniqueDynamic(this, &UTypeFilterWidget::OnFilterButtonPressed);
	SetFrameText();
}

//Filter by the type of the structures
void UTypeFilterWidget::OnFilterButtonPressed()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(player)) return;
	
	UConstructionComponent* constructionComponent = player->GetConstruction();
	if (!IsValid(constructionComponent)) return;
	
	constructionComponent->UpdateFilterRecipeSlots(type);
}

void UTypeFilterWidget::SetFrameText()
{
	typeTextFrame->SetText(textTypeFrame);
	typeText->SetText(textType);
}

