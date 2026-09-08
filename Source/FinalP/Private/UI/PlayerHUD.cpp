// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "ProjectGameMode.h"
#include "UI/PlayerWidget.h"
#include "UI/Construction/StructureMenuWidget.h"
#include "UI/Inventory/ContainerInventory/ContainerWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(playerWidgetClass)) return;
	if (!IsValid(constructionMenuWidgetClass)) return;
	if (!IsValid(inventoryWidgetClass)) return;
	if (!IsValid(containerWidgetClass)) return;
	if (!IsValid(pauseWidgetClass)) return;

	playerWidget = CreateWidget<UPlayerWidget>(GetWorld(), playerWidgetClass);
	constructionMenuWidget = CreateWidget<UStructureMenuWidget>(GetWorld(), constructionMenuWidgetClass);
	inventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), inventoryWidgetClass);
	containerWidget = CreateWidget<UContainerWidget>(GetWorld(), containerWidgetClass);
	pauseWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), pauseWidgetClass);
	
	playerWidget->AddToViewport();
	
	constructionMenuWidget->AddToViewport();
	constructionMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	inventoryWidget->AddToViewport();
	inventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	containerWidget->AddToViewport();
	containerWidget->SetVisibility(ESlateVisibility::Collapsed);

	pauseWidget->AddToViewport();
	pauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	
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
		if (inventoryWidget->IsVisible()) return;
		if (containerWidget->IsVisible()) return;
		if (pauseWidget->IsVisible()) return;
		
		SetPlayerWidgetVisible(false);
		constructionMenuWidget->SetVisibility(ESlateVisibility::Visible);

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		//controller->SetPause(true);
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
		//controller->SetPause(false);
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
		if (pauseWidget->IsVisible()) return;
		if (containerWidget->IsVisible()){ OpenCloseContainerInventory(); return;}
		if (constructionMenuWidget->IsVisible()) return;
		
		SetPlayerWidgetVisible(false);
		inventoryWidget->SetVisibility(ESlateVisibility::Visible);
		inventoryWidget->RemoveSlots();
		inventoryWidget->ItemSlotCreation();

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		//controller->SetPause(true);
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
		//controller->SetPause(false);
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

void APlayerHUD::OpenClosePauseMenu()
{
	if (!IsValid(playerWidget)) return;
	if (!IsValid(pauseWidget)) return;

	APlayerController* controller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(controller)) return;

	if (!pauseWidget->IsVisible())
	{
		if (inventoryWidget->IsVisible()){ OpenCloseInventory(); return;}
		if (containerWidget->IsVisible()){ OpenCloseContainerInventory(); return;}
		if (constructionMenuWidget->IsVisible()){ OpenCloseConstructionMenu(); return;}
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

void APlayerHUD::OpenCloseContainerInventory(APlayerCharacter* player, AOrganizationConstruct* container)
{
	if (!IsValid(playerWidget)) return;
	if (!IsValid(containerWidget)) return;
	if (bMachineWidgetOpen) return;

	APlayerController* controller = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(controller)) return;

	if (!containerWidget->IsVisible())
	{
		if (inventoryWidget->IsVisible()) return;
		if (pauseWidget->IsVisible()) return;
		if (constructionMenuWidget->IsVisible()) return;
		
		if (!IsValid(player)) return;
		if (!IsValid(container)) return;
		SetPlayerWidgetVisible(false);
		containerWidget->SetVisibility(ESlateVisibility::Visible);
		containerWidget->RemoveSlots();
		containerWidget->ItemSlotCreation(player, container);

		controller->SetShowMouseCursor(true);
		controller->SetInputMode(FInputModeGameAndUI());
		//controller->SetPause(true);
	}
	else
	{
		containerWidget->SetVisibility(ESlateVisibility::Collapsed);
		if (!IsAnyMenuOpen())
		{
			SetPlayerWidgetVisible(true);
		}

		controller->SetShowMouseCursor(false);
		controller->SetInputMode(FInputModeGameOnly());
		//controller->SetPause(false);
	}
	
}
