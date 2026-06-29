// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Construction/ConstructionData.h"
#include "Math/TransformCalculus3D.h"
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
	EStructureType defaultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInterface> transparentMaterial{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInterface> transparentBadMaterial{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConstructionData> structureData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDataTable> dataTable{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AConstructionPart> actBuilding{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> previewMesh{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class APlayerCharacter> player{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int actLevel;

	UPROPERTY(EditAnywhere)
	bool buildable;
	
private:
	FConstructionData* dataRow = nullptr;

	TSubclassOf<AConstructionPart> actSelectedPart;

	TArray<FName> unlockedStructuresData;
	
	FVector structurePlace;

	FRotator structureRotator{0,0,0};
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void LoadRecipes();

	UFUNCTION()
	void GetCraftablesStructures(FName rowName);
	
	UFUNCTION()
	void PrintBPsInventory();

	UFUNCTION()
	void UpdateRecipeSlots();
	void UpdateFilterRecipeSlots(EStructureType type);
	
	UFUNCTION()//debug
	void PrintUnlockedRecipes();

	//Construction functions

	UFUNCTION()
	void CreateStructure(TSubclassOf<AConstructionPart>& constructionPart);

	UFUNCTION()
	void CreateGhost();

	UFUNCTION()
	void MoveStructure();

	UFUNCTION()
	bool CheckOverlap();

	UFUNCTION()
	void AvailableColor(bool canBuild);
	
	UFUNCTION()
	void RotateLeftStructure();
	
	UFUNCTION()
	void RotateRightStructure();
	
	UFUNCTION()
	void PlaceStructure();
	
	UFUNCTION()
	void EndBuild();
	
};
