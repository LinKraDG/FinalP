// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UContainerWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class UPanelWidget> playerItemsPanel{};
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<class UPanelWidget> containerItemsPanel{};

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> playerToContainerButton{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> containerToPlayerButton{};

	UFUNCTION()
	void SlotItemsCreation();

	UFUNCTION()
	void PrintPlayerInventory();

	UFUNCTION()
	void PrintContainerInventory();
	
};
