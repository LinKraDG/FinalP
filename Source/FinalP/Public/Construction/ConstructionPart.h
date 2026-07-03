// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/ConstructionData.h"
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
private:
	bool validConstruct;
	int overlapCounter;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> boxCollision{};

	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInterface> originalMaterial{};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> baseMeshComponent{};

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void EndConstruction();
	
	UFUNCTION(BlueprintCallable)
	void SetTransparentMaterial();
	
	UFUNCTION(BlueprintCallable)
	void ChangeColl();

	UFUNCTION()
	UBoxComponent* GetBoxCollisionComponent();

	UFUNCTION()
	void SetPlace(FVector place);

	UFUNCTION()
	bool GetValidConstruct();

	UFUNCTION()
	UStaticMeshComponent* GetStructureMesh();
};
