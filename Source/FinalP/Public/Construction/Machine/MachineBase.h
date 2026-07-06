#pragma once

#include "CoreMinimal.h"
#include "Construction/ConstructionPart.h"
#include "Interfaces/Interactive.h"
#include "Interfaces/MachineWidgetInterface.h"
#include "MachineBase.generated.h"

class UUserWidget;
class URuneSlotComponent;
class UMachinePort;

UENUM(BlueprintType)
enum class EMachineState : uint8 {
	Idle,
	Working,
	OutFull,
	PowerOff,
	NoMagicEnergy
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMachineStateChange, EMachineState, newState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMachineInventoryChange);

UCLASS()
class FINALP_API AMachineBase : public AConstructionPart, public IInteractive
{
	GENERATED_BODY()

public:
	AMachineBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Machine Info")
	FString machineName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Machine Info")
	float processTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Machine Info")
	int outputSpace = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Machine Info")
	TSubclassOf<UUserWidget> machineWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Machine Info")
	TObjectPtr<URuneSlotComponent> runeSlot{};

	UFUNCTION(BlueprintCallable)
	bool InsertRune(FItemData item);

	UPROPERTY()
	FOnMachineStateChange OnMachineStateChange;

	UPROPERTY()
	FOnMachineInventoryChange OnMachineInventoryChange;

	virtual void Interact_Implementation(AActor* actor) override;

	UFUNCTION(BlueprintCallable)
	void OpenWidget();

	UFUNCTION(BlueprintCallable)
	void CloseWidget();

	UFUNCTION(BlueprintCallable)
	void TurnOn();

	UFUNCTION(BlueprintCallable)
	void TurnOff();

	UFUNCTION(BlueprintCallable)
	EMachineState GetMachineState() const { return machineState; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanProcess();
	virtual bool CanProcess_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Process();
	virtual void Process_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual bool ReceiveItem(FItemData item);

	UFUNCTION(BlueprintCallable)
	bool TryExitItem(FItemData& exitItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMachinePort> inputPort{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMachinePort> outputPort{};

	UFUNCTION(BlueprintPure)
	const TArray<FItemData>& GetInputInventory() const { return inputInventory; }

	UFUNCTION(BlueprintPure)
	const TArray<FItemData>& GetOutputInventory() const { return outputInventory; }

	UFUNCTION(BlueprintPure)
	float GetProcessProgress01() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine Info")
	EMachineState machineState{ EMachineState::Idle };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine Info")
	TArray<FItemData> inputInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Machine Info")
	TArray<FItemData> outputInventory;

	UPROPERTY()
	TObjectPtr<UUserWidget> activeWidget{};

	FTimerHandle processTimerHandle;

	void TryStartProcessing();

	void SetMachineState(EMachineState newState);

	UFUNCTION()
	void ProcessComplete();

	UFUNCTION()
	void ResolveEnergyStateChanged(bool bInHasEnergy);

};
