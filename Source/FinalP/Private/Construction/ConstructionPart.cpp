// Fill out your copyright notice in the Description page of Project Settings.


#include "Construction/ConstructionPart.h"

#include "Components/BoxComponent.h"

// Sets default values
AConstructionPart::AConstructionPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	baseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	
	RootComponent = boxCollision;
	baseMeshComponent->SetupAttachment(RootComponent);
}

void AConstructionPart::SetTransparentMaterial()
{
	baseMeshComponent->SetMaterial(0, transparentMaterial);
}

void AConstructionPart::ChangeMaterial()
{
	baseMeshComponent->SetMaterial(0, originalMaterial);
}

// Called when the game starts or when spawned
void AConstructionPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AConstructionPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/



