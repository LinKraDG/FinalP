// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/InventoryItem.h"
#include "Structs/ItemData.h"
#include "InventoryComponent.generated.h"

USTRUCT()
struct FHollowInfo
{
	GENERATED_BODY()

	UPROPERTY()
	bool isEmpty;
	
	UPROPERTY()
	int index;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAmountChanged, FInventoryItem);
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
UCLASS(Blueprintable)
class FINALP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();
	
	FOnItemAmountChanged OnItemAmountChanged;
	FOnInventoryUpdated OnInventoryUpdated;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int/*inventory position*/, FInventoryItem> inventoryData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDataTable> itemDataTable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int actSize;

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	int GetSize();
	
	UFUNCTION()
	void LoadItem(int ID, int amount);

	UFUNCTION(BlueprintCallable)
	int32 AddItem(FItemData item, int32 amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 itemId, int32 amount = 1);

	UFUNCTION()
	void UnloadItem(FItemData item, int index);

	UFUNCTION()
	FItemData GetItem(int index);

	UFUNCTION()
	int GetItemAmount(int ID);

	UFUNCTION()
	void PrintInventory();

	UFUNCTION()
	void NotifyChanges(FInventoryItem item);
};
