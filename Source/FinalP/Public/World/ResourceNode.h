// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactive.h"
#include "ResourceNode.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class FINALP_API AResourceNode : public AActor, public IInteractive
{
	GENERATED_BODY()

public:
	AResourceNode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceNode")
	int32 getPerInteract = 1 ;

	virtual void Interact_Implementation(AActor* actor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ResourceNode")
	TObjectPtr<UStaticMeshComponent> mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceNode")
	FName resourceItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceNode")
	bool bInfinite = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResourceNode")
	int32 amountRemaining = 10;

	UFUNCTION()
	FName GetResourceItemId() const { return resourceItemId; }

	UFUNCTION()
	bool IsEmpty() const { return !bInfinite && amountRemaining <= 0; }

	UFUNCTION()
	bool IsOccupied() const { return bOccupied; }

	UFUNCTION(BlueprintCallable)
	bool PutMachine();

	UFUNCTION(BlueprintCallable)
	void ReleaseMachine();

	UFUNCTION(BlueprintCallable, Category = "ResourceNode")
	int32 ExtractUnits(int32 numMats);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ResourceNode")
	bool bOccupied = false;
};
