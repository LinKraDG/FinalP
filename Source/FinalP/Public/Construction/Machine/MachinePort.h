// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MachinePort.generated.h"

class AMachineBase;
class AConveyorBelt;

UENUM(BlueprintType)
enum class EPortDir : uint8
{
	Input,
	Output
};

UCLASS( ClassGroup=(Magtory), meta=(BlueprintSpawnableComponent) )
class FINALP_API UMachinePort : public USceneComponent
{
	GENERATED_BODY()

public:	
	UMachinePort();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Port")
	EPortDir direction;

	UFUNCTION(BlueprintPure, Category = "Port")
	bool IsConnected() const { return connectedBelt.IsValid(); }

	UFUNCTION(BlueprintCallable, Category = "Port")
	void SetConnectedBelt(AConveyorBelt* belt);

	UFUNCTION()
	AConveyorBelt* GetConnectedBelt() const;

	UFUNCTION()
	AMachineBase* GetMachineAsociated() const;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<AConveyorBelt> connectedBelt;
};
