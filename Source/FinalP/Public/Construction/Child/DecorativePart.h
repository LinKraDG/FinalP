// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Construction/ConstructionPart.h"
#include "DecorativePart.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FINALP_API ADecorativePart : public AConstructionPart
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADecorativePart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
};
