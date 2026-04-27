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
	
public:
	UFUNCTION(BlueprintCallable)
	UPanelWidget* GetItemsPanel();

	UFUNCTION(BlueprintCallable)
	void ShowHideOptions(int id);
};
