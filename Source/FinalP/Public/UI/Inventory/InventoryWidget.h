// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class UPanelWidget> itemsPanel{};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUnicItem> itemsWidget;
	
public:

	UFUNCTION(BlueprintCallable)
	void ShowHideOptions(int id);

	UFUNCTION()
	void ItemSlotCreation();

	UFUNCTION()
	void CreateItemSlot(int slotID, UDataTable* itemsDataTable, int itemID, int itemQuantity);

	UFUNCTION()
	void CreateEmptyItemSlot(int slotID);

	UFUNCTION()
	void RemoveSlots();
};
