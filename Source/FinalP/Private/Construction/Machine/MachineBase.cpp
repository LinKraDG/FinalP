#include "Construction/Machine/MachineBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "RuneSlotComponent.h"
#include "Construction/Machine/MachinePort.h"
#include "TimerManager.h"
#include "UI/PlayerHUD.h"

AMachineBase::AMachineBase()
{
	PrimaryActorTick.bCanEverTick = false;

	runeSlot = CreateDefaultSubobject<URuneSlotComponent>(TEXT("RuneSlot"));

	inputPort = CreateDefaultSubobject<UMachinePort>(TEXT("InputPort"));
	inputPort->SetupAttachment(baseMeshComponent);
	inputPort->direction = EPortDir::Input;

	outputPort = CreateDefaultSubobject<UMachinePort>(TEXT("OutputPort"));
	outputPort->SetupAttachment(baseMeshComponent);
	outputPort->direction = EPortDir::Output;
}

void AMachineBase::BeginPlay()
{
	if (runeSlot){
		runeSlot->OnEnergyStateChange.AddDynamic(this, &AMachineBase::ResolveEnergyStateChanged);
	}

	Super::BeginPlay();

	TryStartProcessing();
}

void AMachineBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld()){
		GetWorld()->GetTimerManager().ClearTimer(processTimerHandle);
	}

	if (activeWidget){
		activeWidget->RemoveFromParent();
		activeWidget = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}


void AMachineBase::Interact_Implementation(AActor* actor)
{
	if (activeWidget && activeWidget->IsVisible()){
		CloseWidget();
	}
	else{
		OpenWidget();
	}
}

void AMachineBase::OpenWidget()
{
	if (!machineWidgetClass){return;}
	if (activeWidget && activeWidget->IsVisible()){return;}

	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	if (!controller){return;}

	APlayerHUD* hud = Cast<APlayerHUD>(controller->GetHUD());
	if (hud && hud->IsAnyMenuOpen()){return;}

	if (!activeWidget){
		activeWidget = CreateWidget<UUserWidget>(controller, machineWidgetClass);
		if (!activeWidget){return;}

		if (activeWidget->GetClass()->ImplementsInterface(UMachineWidgetInterface::StaticClass())){
			IMachineWidgetInterface::Execute_SetMachine(activeWidget, this);
		}
		activeWidget->AddToViewport();
	}
	else{
		activeWidget->SetVisibility(ESlateVisibility::Visible);
	}

	controller->SetShowMouseCursor(true);
	controller->SetInputMode(FInputModeGameAndUI());

	if (hud){
		hud->NotifyMachineWidgetOpened();
	}
}

void AMachineBase::CloseWidget()
{
	if (!activeWidget || !activeWidget->IsVisible()){return;}

	activeWidget->SetVisibility(ESlateVisibility::Collapsed);

	if (APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0)){
		controller->SetShowMouseCursor(false);
		controller->SetInputMode(FInputModeGameOnly());

		if (APlayerHUD* hud = Cast<APlayerHUD>(controller->GetHUD())){
			hud->NotifyMachineWidgetClosed();
		}
	}
}

void AMachineBase::TurnOn()
{
	if (machineState == EMachineState::PowerOff){
		SetMachineState(EMachineState::Idle);
		TryStartProcessing();
	}
}

void AMachineBase::TurnOff()
{
	if (GetWorld()){
		GetWorld()->GetTimerManager().ClearTimer(processTimerHandle);
	}
	SetMachineState(EMachineState::PowerOff);
}

bool AMachineBase::CanProcess_Implementation()
{
	return false;
}

void AMachineBase::Process_Implementation()
{

}

void AMachineBase::TryStartProcessing()
{
	if (machineState == EMachineState::PowerOff || !GetWorld()){return;}

	if (runeSlot && runeSlot->requiresEnergy && !runeSlot->HasEnergy()){
		SetMachineState(EMachineState::NoMagicEnergy);
		return;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(processTimerHandle)){return;}

	if (CanProcess()){
		SetMachineState(EMachineState::Working);
		GetWorld()->GetTimerManager().SetTimer(
			processTimerHandle, this, &AMachineBase::ProcessComplete, processTime, false);
	}
	else{
		SetMachineState(EMachineState::Idle);
	}
}

void AMachineBase::ProcessComplete()
{
	if (GetWorld()){
		GetWorld()->GetTimerManager().ClearTimer(processTimerHandle);
	}

	Process();

	OnMachineInventoryChange.Broadcast();

	TryStartProcessing();
}

void AMachineBase::ResolveEnergyStateChanged(bool bInHasEnergy)
{
	if (machineState == EMachineState::PowerOff){return;}

	if (bInHasEnergy){
		if (machineState == EMachineState::NoMagicEnergy){
			SetMachineState(EMachineState::Idle);
		}
		TryStartProcessing();
	}
	else{
		if (runeSlot && runeSlot->requiresEnergy){
			if (GetWorld()){
				GetWorld()->GetTimerManager().ClearTimer(processTimerHandle);
			}
			SetMachineState(EMachineState::NoMagicEnergy);
		}
	}
}

bool AMachineBase::InsertRune(FItemData item)
{
	return runeSlot && runeSlot->InsertRune(item, 1);
}

bool AMachineBase::ReceiveItem(FItemData item)
{
	if (machineState == EMachineState::PowerOff){
		return false;
	}

	inputInventory.Add(item);
	OnMachineInventoryChange.Broadcast();

	TryStartProcessing();
	return true;
}

bool AMachineBase::TryExitItem(FItemData& exitItem)
{
	if (outputInventory.Num() == 0){
		return false;
	}

	exitItem = outputInventory[0];
	outputInventory.RemoveAt(0);
	OnMachineInventoryChange.Broadcast();

	TryStartProcessing();
	return true;
}

float AMachineBase::GetProcessProgress01() const
{
	if (machineState != EMachineState::Working || !GetWorld()){
		return 0.f;
	}

	const FTimerManager& timerManager = GetWorld()->GetTimerManager();
	if (!timerManager.IsTimerActive(processTimerHandle) || processTime <= 0.f){
		return 0.f;
	}

	const float elapsed = timerManager.GetTimerElapsed(processTimerHandle);
	return FMath::Clamp(elapsed / processTime, 0.f, 1.f);
}

void AMachineBase::SetMachineState(EMachineState newState)
{
	if (machineState != newState){
		machineState = newState;
		OnMachineStateChange.Broadcast(newState);
	}
}
