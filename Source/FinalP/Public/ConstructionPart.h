// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConstructionPart.generated.h"

UCLASS()
class FINALP_API AConstructionPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConstructionPart();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMaterialInterface> OriginalMaterial{};
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> BaseMeshComponent{};
	
	UFUNCTION(BlueprintImplementableEvent)
	void EndConstruction();
	
	UFUNCTION(BlueprintCallable)
	void ChangeMaterial();
};
