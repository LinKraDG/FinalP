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

	playerWidget = CreateWidget<UPlayerWidget>(GetWorld(), playerWidgetClass);
	constructionMenuWidget = CreateWidget<UStructureMenuWidget>(GetWorld(), constructionMenuWidgetClass);
	inventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), inventoryWidgetClass);
	
	playerWidget->AddToViewport();
	
	constructionMenuWidget->AddToViewport();
	constructionMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	inventoryWidget->AddToViewport();
	inventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	
}

void APlayerHUD::OpenCloseConstructionMenu()
{
	if (!IsValid(playerWidget)) return;
	if (!IsValid(constructionMenuWidget)) return;
	if (bMachineWidgetOpen) return;

	APlayerController* controller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(controller)) return;

	if (!constructionMenuWidget->IsVisible())
	{
		SetPlayerWidgetVisible(false);
		constructionMenuWidget->SetVisibility(ESlateVisibility::Visible);

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		controller->SetPause(true);
	}
	else
	{
		constructionMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		if (!IsAnyMenuOpen())
		{
			SetPlayerWidgetVisible(true);
		}

		controller->SetShowMouseCursor(false);
		controller->SetInputMode(FInputModeGameOnly());
		controller->SetPause(false);
	}
}

void APlayerHUD::OpenCloseInventory()
{
	if (!IsValid(playerWidget)) return;
	if (!IsValid(inventoryWidget)) return;
	if (bMachineWidgetOpen) return;

	APlayerController* controller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(controller)) return;

	if (!inventoryWidget->IsVisible())
	{
		SetPlayerWidgetVisible(false);
		inventoryWidget->SetVisibility(ESlateVisibility::Visible);

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		controller->SetPause(true);
	}
	else
	{
		inventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		if (!IsAnyMenuOpen())
		{
			SetPlayerWidgetVisible(true);
		}

		controller->SetShowMouseCursor(false);
		controller->SetInputMode(FInputModeGameOnly());
		controller->SetPause(false);
	}
}

void APlayerHUD::SetPlayerWidgetVisible(bool bVisible)
{
	if (!IsValid(playerWidget)) return;

	playerWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void APlayerHUD::NotifyMachineWidgetOpened()
{
	bMachineWidgetOpen = true;
	SetPlayerWidgetVisible(false);
}

void APlayerHUD::NotifyMachineWidgetClosed()
{
	bMachineWidgetOpen = false;
	if (!IsAnyMenuOpen())
	{
		SetPlayerWidgetVisible(true);
	}
}

bool APlayerHUD::IsAnyMenuOpen() const
{
	return (IsValid(constructionMenuWidget) && constructionMenuWidget->IsVisible())
		|| (IsValid(inventoryWidget) && inventoryWidget->IsVisible())
		|| bMachineWidgetOpen;
}

UPlayerWidget* APlayerHUD::GetPlayerWidget()
{
	return playerWidget;
}

UInventoryWidget* APlayerHUD::GetInventoryWidget()
{
	return inventoryWidget;
}

UStructureMenuWidget* APlayerHUD::GetStructureWidget()
{
	return constructionMenuWidget;
}
