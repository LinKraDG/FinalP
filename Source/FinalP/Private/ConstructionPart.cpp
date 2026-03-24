// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionPart.h"

// Sets default values
AConstructionPart::AConstructionPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	RootComponent = BaseMeshComponent;
}

void AConstructionPart::ChangeMaterial()
{
	BaseMeshComponent->SetMaterial(0, OriginalMaterial);
}

// Called when the game starts or when spawned
void AConstructionPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConstructionPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



