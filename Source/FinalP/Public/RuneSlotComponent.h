// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/ItemData.h"
#include "Structs/InventoryItem.h"
#include "RuneSlotComponent.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyStateChange, bool, bHasEnergy);

UCLASS( ClassGroup=(Energy), meta=(BlueprintSpawnableComponent) )
class FINALP_API URuneSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URuneSlotComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	bool requiresEnergy = false;

	UPROPERTY(BlueprintAssignable, Category = "Energy")
	FOnEnergyStateChange OnEnergyStateChange;

	UFUNCTION(BlueprintCallable)
	bool InsertRune(FItemData item, int32 amount);

	UFUNCTION(BlueprintPure)
	bool HasEnergy() const { return bHasEnergy; }

	UFUNCTION(BlueprintPure)
	float GetRuneRemainingDuration() const;

	UFUNCTION(BlueprintPure)
	int32 GetStoredRuneCount() const { return insertedRune.Amount; }

	UFUNCTION(BlueprintPure)
	const FInventoryItem& GetInsertedRune() const { return insertedRune; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Energy")
	bool bHasEnergy = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Energy")
	float energyDuration = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Energy")
	float currentEnergyRemaining = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Energy")
	FInventoryItem insertedRune;

	UPROPERTY(EditAnywhere, Category = "Energy")
	TObjectPtr<UDataTable> itemDataTable{};

	bool FindRuneDefinition(int32 itemId, FItemData& outItem) const;

	bool UseNextRuneStack();

	void StopEnergy();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
