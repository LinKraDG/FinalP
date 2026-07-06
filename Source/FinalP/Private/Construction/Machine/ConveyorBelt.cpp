#include "Construction/Machine/ConveyorBelt.h"
#include "Construction/Machine/MachineBase.h"
#include "Construction/Machine/MachinePort.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/Engine.h"

AConveyorBelt::AConveyorBelt()
{
	PrimaryActorTick.bCanEverTick = true;

	beltPath = CreateDefaultSubobject<USplineComponent>(TEXT("BeltPath"));
	beltPath->SetupAttachment(baseMeshComponent);

	itemInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ItemInstances"));
	itemInstances->SetupAttachment(baseMeshComponent);
	itemInstances->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	itemInstances->SetCastShadow(false);

	beltMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("BeltMesh"));
	beltMesh->SetupAttachment(baseMeshComponent);
	beltMesh->SetMobility(EComponentMobility::Movable);
	beltMesh->SetForwardAxis(ESplineMeshAxis::X);
	beltMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	beltMesh->SetVisibility(false);
}

void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();

	if (itemInstances && itemMesh){
		itemInstances->SetStaticMesh(itemMesh);
	}
}

void AConveyorBelt::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Disconnect();

	Super::EndPlay(EndPlayReason);
}

void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bConnected){
		AdvanceItems(DeltaTime);
		TryToDestination();
		TryPullFromSource();
	}

	UpdateItemVisuals();
}

AConveyorBelt* AConveyorBelt::ConnectMachines(UObject* WorldContextObject, TSubclassOf<AConveyorBelt> beltClass,AMachineBase* source, AMachineBase* destination)
{
	if (!IsValid(source) || !IsValid(destination) || source == destination || !beltClass){return nullptr;}

	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!world){return nullptr;}

	const FVector spawnLocation = source->outputPort ? source->outputPort->GetComponentLocation() : source->GetActorLocation();
	AConveyorBelt* belt = world->SpawnActor<AConveyorBelt>(beltClass, spawnLocation, FRotator::ZeroRotator);
	if (!IsValid(belt)){return nullptr;}

	if (!belt->ConnectToMachines(source, destination)){
		belt->Destroy();
		return nullptr;
	}

	return belt;
}

bool AConveyorBelt::ConnectToMachines(AMachineBase* source, AMachineBase* destination)
{
	if (!IsValid(source) || !IsValid(destination) || source == destination){return false;}

	UMachinePort* outPort = source->outputPort;
	UMachinePort* inPort = destination->inputPort;
	if (!outPort || !inPort){return false;}

	sourcePort = outPort;
	destPort = inPort;
	sourceMachine = source;
	destinationMachine = destination;

	const FVector worldStart = outPort->GetComponentLocation();
	const FVector worldEnd = inPort->GetComponentLocation();

	if (beltPath){
		beltPath->ClearSplinePoints(false);
		beltPath->AddSplinePoint(worldStart, ESplineCoordinateSpace::World, false);
		beltPath->AddSplinePoint(worldEnd, ESplineCoordinateSpace::World, false);
		beltPath->UpdateSpline();
	}

	UpdateBeltMesh(worldStart, worldEnd);

	return Connect();
}

void AConveyorBelt::UpdateBeltMesh(const FVector& worldStart, const FVector& worldEnd)
{
	if (!beltMesh){return;}

	if (beltSegmentMesh && beltMesh->GetStaticMesh() != beltSegmentMesh){
		beltMesh->SetStaticMesh(beltSegmentMesh);
	}

	const FTransform toLocal = GetActorTransform().Inverse();
	const FVector localStart = toLocal.TransformPosition(worldStart);
	const FVector localEnd = toLocal.TransformPosition(worldEnd);

	const FVector localTangent = toLocal.TransformVector(worldEnd - worldStart);

	beltMesh->SetStartAndEnd(localStart, localTangent, localEnd, localTangent);
	beltMesh->SetVisibility(true);
}

bool AConveyorBelt::Connect()
{
	if (!IsValid(sourceMachine) || !IsValid(destinationMachine) || sourceMachine == destinationMachine){return false;}
	if (bConnected){return true;}

	bConnected = true;
	return true;
}

void AConveyorBelt::Disconnect()
{
	sourcePort = nullptr;
	destPort = nullptr;

	sourceMachine = nullptr;
	destinationMachine = nullptr;

	if (beltMesh){
		beltMesh->SetVisibility(false);
	}

	travelingItems.Reset();
	if (itemInstances && itemInstances->GetInstanceCount() > 0){
		itemInstances->ClearInstances();
	}

	bConnected = false;
}

void AConveyorBelt::AdvanceItems(float DeltaTime)
{
	if (travelingItems.Num() == 0 || !beltPath){return;}

	const float length = beltPath->GetSplineLength();
	if (length <= 0.f){
		return;
	}

	const float step = beltSpeed * 100.f * DeltaTime;

	for (int32 i = 0; i < travelingItems.Num(); ++i){
		const float maxAllowed = (i == 0) ? length : FMath::Max(0.f, travelingItems[i - 1].distance - itemSpacing);
		travelingItems[i].distance = FMath::Min(travelingItems[i].distance + step, maxAllowed);
	}
}

void AConveyorBelt::TryToDestination()
{
	if (travelingItems.Num() == 0 || !beltPath){
		return;
	}

	const float length = beltPath->GetSplineLength();

	FBeltItem& front = travelingItems[0];
	if (front.distance < length - 0.5f){return;}

	if (IsValid(destinationMachine) && destinationMachine->ReceiveItem(front.data))
	{
		travelingItems.RemoveAt(0);
	}
}

void AConveyorBelt::TryPullFromSource()
{
	if (!IsValid(sourceMachine)){return;}

	if (travelingItems.Num() > 0 && travelingItems.Last().distance < itemSpacing){return;}

	FItemData popped;
	if (sourceMachine->TryExitItem(popped)){
		FBeltItem newItem;
		newItem.data = popped;
		newItem.distance = 0.f;
		travelingItems.Add(newItem);
	}
}

void AConveyorBelt::UpdateItemVisuals()
{
	if (!itemInstances || !beltPath){return;}

	if (!itemMesh || travelingItems.Num() == 0){
		if (itemInstances->GetInstanceCount() > 0)
		{
			itemInstances->ClearInstances();
		}
		return;
	}

	if (itemInstances->GetStaticMesh() != itemMesh){
		itemInstances->SetStaticMesh(itemMesh);
	}

	while (itemInstances->GetInstanceCount() < travelingItems.Num()){
		itemInstances->AddInstance(FTransform::Identity);
	}
	while (itemInstances->GetInstanceCount() > travelingItems.Num()){
		itemInstances->RemoveInstance(itemInstances->GetInstanceCount() - 1);
	}

	for (int32 i = 0; i < travelingItems.Num(); ++i){
		const FTransform worldT = beltPath->GetTransformAtDistanceAlongSpline(
			travelingItems[i].distance, ESplineCoordinateSpace::World);

		const bool bLast = (i == travelingItems.Num() - 1);
		itemInstances->UpdateInstanceTransform(i, worldT, true,bLast,true);
	}
}
