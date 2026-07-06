// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryWidget.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
class UPlayerWidget;
class UStructureMenuWidget;

UCLASS()
class FINALP_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UPlayerWidget* playerWidget = nullptr;
	
	UPROPERTY()
	UStructureMenuWidget* constructionMenuWidget = nullptr;
	
	UPROPERTY()
	UInventoryWidget* inventoryWidget = nullptr;

	bool bMachineWidgetOpen = false;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	TSubclassOf<UPlayerWidget> playerWidgetClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	TSubclassOf<UStructureMenuWidget> constructionMenuWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	TSubclassOf<UInventoryWidget> inventoryWidgetClass = nullptr;

	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
	void OpenCloseConstructionMenu();

	UFUNCTION(BlueprintCallable)
	void OpenCloseInventory();

	UFUNCTION(BlueprintCallable)
	void SetPlayerWidgetVisible(bool bVisible);

	UFUNCTION(BlueprintCallable)
	void NotifyMachineWidgetOpened();

	UFUNCTION(BlueprintCallable)
	void NotifyMachineWidgetClosed();

	UFUNCTION(BlueprintPure)
	bool IsAnyMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	UPlayerWidget* GetPlayerWidget();

	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget();

	UFUNCTION(BlueprintCallable)
	UStructureMenuWidget* GetStructureWidget();
};
