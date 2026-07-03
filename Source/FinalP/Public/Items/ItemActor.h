// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/ItemData.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactive.h"
#include "ItemActor.generated.h"

UCLASS()
class FINALP_API AItemActor : public AActor, public IInteractive
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<class UStaticMeshComponent> staticMesh{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FItemData itemData{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class APlayerCharacter> player{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int quantity;
	
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* actor) override;

	UFUNCTION()
	void ReduceQuantity(int extract);

};