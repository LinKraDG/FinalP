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
	boxCollision->InitBoxExtent(FVector{1.f});
	
	
	//RootComponent = boxCollision;
	baseMeshComponent->SetupAttachment(RootComponent);
	boxCollision->SetupAttachment(baseMeshComponent);

	overlapCounter = 0;
}

void AConstructionPart::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	overlapCounter++;
	validConstruct = false;
	//baseMeshComponent->SetMaterial(0, transparentBadMaterial);
}

void AConstructionPart::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	overlapCounter--;
	if (overlapCounter == 0)
	{
		validConstruct = true;
		//baseMeshComponent->SetMaterial(0, transparentMaterial);
	}
}

void AConstructionPart::SetTransparentMaterial()
{
	validConstruct = true;
	//baseMeshComponent->SetMaterial(0, transparentMaterial);
	//SetActorEnableCollision(false);
	//boxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	//boxCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxCollision->BodyInstance.SetCollisionProfileName("UnplacedStructure");
	baseMeshComponent->BodyInstance.SetCollisionProfileName("UnplacedStructure");

}

void AConstructionPart::ChangeColl()
{
	//baseMeshComponent->SetMaterial(0, originalMaterial);
	
	boxCollision->BodyInstance.SetCollisionProfileName("BlockAll");
	baseMeshComponent->BodyInstance.SetCollisionProfileName("BlockAll");
	
	boxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AConstructionPart::BeginOverlap);
	boxCollision->OnComponentEndOverlap.RemoveDynamic(this, &AConstructionPart::EndOverlap);
}

UBoxComponent* AConstructionPart::GetBoxCollisionComponent()
{
	return boxCollision;
}

void AConstructionPart::SetPlace(FVector place)
{
	SetActorLocation(place);
	//if (boxCollision->)
}

bool AConstructionPart::GetValidConstruct()
{
	return validConstruct;
}

UStaticMeshComponent* AConstructionPart::GetStructureMesh()
{
	return baseMeshComponent;
}

// Called when the game starts or when spawned
void AConstructionPart::BeginPlay()
{
	Super::BeginPlay();

	//boxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &AConstructionPart::BeginOverlap);
	//boxCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &AConstructionPart::EndOverlap);
}

// Called every frame
/*void AConstructionPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/



