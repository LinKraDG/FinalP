// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Construction/ConstructionData.h"
#include "ConstructionComponent.generated.h"


UCLASS(Blueprintable)
class FINALP_API UConstructionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConstructionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConstructionData> structureData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDataTable> dataTable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int actLevel;

private:
	FConstructionData* dataRow = nullptr;

	TArray<FName> unlockedStructuresData;
public:
	// Called every frame
	/*virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;*/

	UFUNCTION()
	void LoadRecipes();

	UFUNCTION()
	void GetCraftablesStructures(FName rowName);
	
	UFUNCTION()
	void PrintBPsInventory();

	UFUNCTION()
	void UpdateRecipeSlots();
	
	UFUNCTION()//debug
	void PrintUnlockedRecipes();
};
