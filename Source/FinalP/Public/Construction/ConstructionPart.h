// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstructionData.h"
#include "GameFramework/Actor.h"
#include "ConstructionPart.generated.h"

UCLASS()
class FINALP_API AConstructionPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConstructionPart();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	//FConstructionData structureData{};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInterface> transparentMaterial{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInterface> originalMaterial{};
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> baseMeshComponent{};
	
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void EndConstruction();
	
	UFUNCTION(BlueprintCallable)
	void SetTransparentMaterial();
	
	UFUNCTION(BlueprintCallable)
	void ChangeMaterial();
};
