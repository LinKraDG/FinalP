// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

UCLASS(Blueprintable)
class FINALP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemData> inventoryData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int actSize;

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	int GetSize();
	
	UFUNCTION()
	FHollowInfo CheckHollow(FItemData data);
	
	UFUNCTION()
	void LoadItem(FItemData item, int index);

	UFUNCTION()
	void UnloadItem(FItemData item, int index);

	UFUNCTION()
	FItemData GetItem(int index);

	UFUNCTION()
	void PrintInventory();
};
