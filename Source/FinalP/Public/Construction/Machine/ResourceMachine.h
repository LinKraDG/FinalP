// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Construction/Machine/MachineBase.h"
#include "ResourceMachine.generated.h"

class AResourceNode;

UCLASS(Blueprintable, BlueprintType)
class FINALP_API AResourceMachine : public AMachineBase
{
	GENERATED_BODY()

public:
	AResourceMachine();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceMachine")
	int amountPerCycle = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceMachine")
	TObjectPtr<AResourceNode> targetNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceMachine")
	float nodeRadius = 200.f;

	virtual bool CanProcess_Implementation() override;

	virtual void Process_Implementation() override;

	virtual bool GetValidConstruct() override;

	virtual void OnPlaced() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	AResourceNode* FindNearestFreeNode() const;
};
