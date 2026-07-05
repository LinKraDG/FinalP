// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "ProjectGameMode.h"
#include "UI/PlayerWidget.h"
#include "UI/Construction/StructureMenuWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(playerWidgetClass)) return;
	if (!IsValid(constructionMenuWidgetClass)) return;
	if (!IsValid(inventoryWidgetClass)) return;
	if (!IsValid(pauseWidgetClass)) return;

	playerWidget = CreateWidget<UPlayerWidget>(GetWorld(), playerWidgetClass);
	constructionMenuWidget = CreateWidget<UStructureMenuWidget>(GetWorld(), constructionMenuWidgetClass);
	inventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), inventoryWidgetClass);
	pauseWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), pauseWidgetClass);
	
	playerWidget->AddToViewport();
	
	constructionMenuWidget->AddToViewport();
	constructionMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	inventoryWidget->AddToViewport();
	inventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

	pauseWidget->AddToViewport();
	pauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	
}

void APlayerHUD::OpenCloseConstructionMenu()
{
	if (!IsValid(playerWidget)) return;
	if (!IsValid(constructionMenuWidget)) return;

	APlayerController* controller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(controller)) return;

	if (playerWidget->IsVisible())
	{
		playerWidget->SetVisibility(ESlateVisibility::Collapsed);
		constructionMenuWidget->SetVisibility(ESlateVisibility::Visible);

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		controller->SetPause(true);
	}
	else
	{
		playerWidget->SetVisibility(ESlateVisibility::Visible);
		constructionMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

		controller->SetShowMouseCursor(false);
		controller->SetInputMode(FInputModeGameOnly());
		controller->SetPause(false);
	}
}

void APlayerHUD::OpenCloseInventory()
{
	if (!IsValid(playerWidget)) return;
	if (!IsValid(inventoryWidget)) return;
	
	APlayerController* controller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(controller)) return;
	
	if (playerWidget->IsVisible())
	{
		playerWidget->SetVisibility(ESlateVisibility::Collapsed);
		inventoryWidget->SetVisibility(ESlateVisibility::Visible);

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		controller->SetPause(true);
	}
	else
	{
		playerWidget->SetVisibility(ESlateVisibility::Visible);
		inventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		controller->SetShowMouseCursor(false);
		controller->SetInputMode(FInputModeGameOnly());
		controller->SetPause(false);
	}
	
}

void APlayerHUD::OpenClosePauseMenu()
{
	if (!IsValid(playerWidget)) return;
	if (!IsValid(pauseWidget)) return;

	APlayerController* controller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(controller)) return;

	if (playerWidget->IsVisible())
	{
		playerWidget->SetVisibility(ESlateVisibility::Collapsed);
		pauseWidget->SetVisibility(ESlateVisibility::Visible);

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		controller->SetPause(true);
	}
	else
	{
		playerWidget->SetVisibility(ESlateVisibility::Visible);
		pauseWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		controller->SetShowMouseCursor(false);
		controller->SetInputMode(FInputModeGameOnly());
		controller->SetPause(false);
	}
}

UInventoryWidget* APlayerHUD::GetInventoryWidget()
{
	return inventoryWidget;
}

UStructureMenuWidget* APlayerHUD::GetStructureWidget()
{
	return constructionMenuWidget;
}
