// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Construction/ConstructionPart.h"
#include "Interfaces/Interactive.h"
#include "OrganizationConstruct.generated.h"

UCLASS()
class FINALP_API AOrganizationConstruct : public AConstructionPart, public IInteractive
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOrganizationConstruct();

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<class UInventoryComponent> inventoryComponent{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* playerCharacter) override;
};
