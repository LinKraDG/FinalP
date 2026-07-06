// Fill out your copyright notice in the Description page of Project Settings.


#include "Construction/Machine/MachinePort.h"
#include "Construction/Machine/MachineBase.h"
#include "Construction/Machine/ConveyorBelt.h"

UMachinePort::UMachinePort()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMachinePort::SetConnectedBelt(AConveyorBelt* belt)
{
	connectedBelt = belt;
}

AConveyorBelt* UMachinePort::GetConnectedBelt() const
{
	return connectedBelt.Get();
}

AMachineBase* UMachinePort::GetMachineAsociated() const
{
	return Cast<AMachineBase>(GetOwner());
}


