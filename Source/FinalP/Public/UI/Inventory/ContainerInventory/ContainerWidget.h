// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/PlayerCharacter.h"
#include "Construction/Child/OrganizationConstruct.h"
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUnicContainerWidget> itemsWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDataTable> itemsDataTable{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class APlayerCharacter> player{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AOrganizationConstruct> container{};

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> playerToContainerButton{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> containerToPlayerButton{};

public:

	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void ItemSlotCreation(APlayerCharacter* playerI, AOrganizationConstruct* containerI);

	UFUNCTION()
	void CreatePlayerItemSlot(int slotID, int itemID, int itemQuantity);

	UFUNCTION()
	void CreateContainerItemSlot(int slotID, int itemID, int itemQuantity);

	UFUNCTION()
	void CreatePlayerEmptyItemSlot(int slotID);

	UFUNCTION()
	void CreateContainerEmptyItemSlot(int slotID);

	UFUNCTION()
	void RemoveSlots();

	//PlayerToContainer
	UFUNCTION()
	void OnPToCButtonPressed();

	//ContainerToPlayer
	UFUNCTION()
	void OnCToPButtonPressed();

	//UFUNCTION()
	void RePrintInventories();
	
};
