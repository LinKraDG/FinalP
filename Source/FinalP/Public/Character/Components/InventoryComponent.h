// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/InventoryItem.h"
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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAmountChanged, const FInventoryItem&);
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
	TMap<int, FInventoryItem> inventoryData;

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
	void LoadItem(FItemData item, int amount);

	UFUNCTION()
	void UnloadItem(FItemData item, int index);

	UFUNCTION()
	FItemData GetItem(int index);

	UFUNCTION()
	void PrintInventory();

	UFUNCTION()
	void NotifyChanges(FInventoryItem item);
};
