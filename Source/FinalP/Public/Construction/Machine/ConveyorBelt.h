// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Construction/ConstructionPart.h"
#include "Construction/Machine/MachinePort.h"
#include "Structs/ItemData.h"
#include "ConveyorBelt.generated.h"

class AMachineBase;
class USplineComponent;
class USplineMeshComponent;
class UInstancedStaticMeshComponent;
class UStaticMesh;
class UMachinePort;

USTRUCT()
struct FBeltItem
{
	GENERATED_BODY()

	UPROPERTY()
	FItemData data;

	UPROPERTY()
	float distance = 0.0f;
};

UCLASS()
class FINALP_API AConveyorBelt : public AConstructionPart
{
	GENERATED_BODY()

public:
	AConveyorBelt();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AMachineBase> sourceMachine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AMachineBase> destinationMachine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float beltSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float itemSpacing = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineComponent> beltPath{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UInstancedStaticMeshComponent> itemInstances{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> itemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USplineMeshComponent> beltMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> beltSegmentMesh;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static AConveyorBelt* ConnectMachines(UObject* WorldContextObject, TSubclassOf<AConveyorBelt> beltClass, AMachineBase* source, AMachineBase* destination);

	UFUNCTION(BlueprintCallable)
	bool ConnectToMachines(AMachineBase* source, AMachineBase* destination);

	UFUNCTION(BlueprintCallable)
	bool Connect();

	UFUNCTION(BlueprintCallable)
	void Disconnect();

	UFUNCTION()
	bool IsConnected() const { return bConnected; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ConveyorBelt")
	bool bConnected = false;

	UPROPERTY(Transient)
	TWeakObjectPtr<UMachinePort> sourcePort;

	UPROPERTY(Transient)
	TWeakObjectPtr<UMachinePort> destPort;

	UPROPERTY(Transient)
	TArray<FBeltItem> travelingItems;

	void UpdateBeltMesh(const FVector& worldStart, const FVector& worldEnd);

	void AdvanceItems(float DeltaTime);

	void TryToDestination();

	void TryPullFromSource();

	void UpdateItemVisuals();
};
