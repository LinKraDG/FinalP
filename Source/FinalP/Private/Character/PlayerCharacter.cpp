// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "UI/PlayerHUD.h"
#include "UI/PlayerWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/ConstructionComponent.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/Components/StaminaComponent.h"
#include "Components/BoxComponent.h"
#include "Construction/ConstructionPart.h"
#include "Construction/Machine/ConveyorBelt.h"
#include "Construction/Machine/MachineBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Interactive.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	constructionComponent = CreateDefaultSubobject<UConstructionComponent>("Construction");
	staminaComponent = CreateDefaultSubobject<UStaminaComponent>("Stamina");

	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 400.f;
	springArm->bUsePawnControlRotation = true;

	camera->SetupAttachment(springArm);
	camera->bUsePawnControlRotation = false;

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void APlayerCharacter::OpenCloseBuildMenu()
{
	if (!IsValid(Controller)) return;

	APlayerController* playerController = Cast<APlayerController>(Controller);

	if (!IsValid(playerController)) return;

	auto * hud = Cast<APlayerHUD>(playerController->GetHUD());
	hud->OpenCloseConstructionMenu();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (!IsValid(playerController)) return;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (!IsValid(subsystem)) return;

	subsystem->AddMappingContext(defaultMappingContext, 0);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(staminaComponent)) return;

	const bool bMoving = GetVelocity().SizeSquared2D() > 100.f;
	const bool bSprinting = bWantsToSprint && bMoving && staminaComponent->CanSprint();

	staminaComponent->UpdateStamina(DeltaTime, bSprinting);

	if (UCharacterMovementComponent* movement = GetCharacterMovement()){
		movement->MaxWalkSpeed = bSprinting ? sprintSpeed : walkSpeed;
	}

	UpdateStaminaUI();
}

void APlayerCharacter::UpdateStaminaUI()
{
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (!IsValid(playerController)) return;

	APlayerHUD* hud = Cast<APlayerHUD>(playerController->GetHUD());
	if (!IsValid(hud)) return;

	UPlayerWidget* widget = hud->GetPlayerWidget();
	if (!IsValid(widget)) return;

	widget->SetStaminaPercent(staminaComponent->GetStaminaPercent());
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EDefaultInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!IsValid(EDefaultInputComponent)) return;
	//Default actions
	EDefaultInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EDefaultInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EDefaultInputComponent->BindAction(orbitalCameraAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OrbitalCameraChange);
	EDefaultInputComponent->BindAction(zoomInCameraAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ZoomInCameraChange);
	EDefaultInputComponent->BindAction(zoomOutCameraAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ZoomOutCameraChange);
	EDefaultInputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EDefaultInputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EDefaultInputComponent->BindAction(sprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
	EDefaultInputComponent->BindAction(sprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
	EDefaultInputComponent->BindAction(buildAction, ETriggerEvent::Started, this, &APlayerCharacter::BuildMenu);
	EDefaultInputComponent->BindAction(interactAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	EDefaultInputComponent->BindAction(inventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::InventoryMenu);
	EDefaultInputComponent->BindAction(LinkAction, ETriggerEvent::Started, this, &APlayerCharacter::SelectMachineForLink);
	EDefaultInputComponent->BindAction(CancelLinkAction, ETriggerEvent::Started, this, &APlayerCharacter::CancelLink);
	//Build actions
	EDefaultInputComponent->BindAction(rotateLeftStructureAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RotateLeftStructure);
	EDefaultInputComponent->BindAction(rotateRightStructureAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RotateRightStructure);
	EDefaultInputComponent->BindAction(placeStructureAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlaceStructure);
	EDefaultInputComponent->BindAction(endBuildAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EndBuild);

	/*UEnhancedInputComponent* EBuildInputComponent = Cast<UEnhancedInputComponent>(buildMappingContext);
	if (!IsValid(EBuildInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("No es valido el build"));
		return;
	}
	EBuildInputComponent->BindAction(changeStructureAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeStructure);
	EBuildInputComponent->BindAction(endBuildAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EndBuild);*/
}

UInventoryComponent* APlayerCharacter::GetInventory()
{
	return inventoryComponent;
}

UConstructionComponent* APlayerCharacter::GetConstruction()
{
	return constructionComponent;
}

void APlayerCharacter::SetConstructionMode(TSubclassOf<AConstructionPart> part)
{
	if (!IsValid(part)) return;
	constructionPart = part;

	constructionComponent->CreateStructure(constructionPart);

	OpenCloseBuildMenu();

	//CreateStructure();
}

void APlayerCharacter::ChangeToBuildMappingContext()
{
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (!IsValid(playerController)) return;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (!IsValid(subsystem)) return;

	subsystem->AddMappingContext(buildMappingContext, 1);
}

void APlayerCharacter::ChangeToDefaultMappingContext()
{
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (!IsValid(playerController)) return;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (!IsValid(subsystem)) return;

	subsystem->RemoveMappingContext(buildMappingContext);
}

void APlayerCharacter::Move(const FInputActionValue& actionValue)
{
	if (!IsValid(Controller)) return;

	if (APlayerController* pc = Cast<APlayerController>(Controller)){
		if (APlayerHUD* hud = Cast<APlayerHUD>(pc->GetHUD())){
			if (hud->IsAnyMenuOpen()) return;
		}
	}

	FVector2D movementVector = actionValue.Get<FVector2D>();

	switch (bUseControllerRotationYaw)
	{
		case true:
			{
				const FRotator rotation = Controller->GetControlRotation();
				const FRotator yawRotation = FRotator(0,rotation.Yaw,0);

				const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
				const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

				AddMovementInput(forwardDirection, movementVector.Y);
				AddMovementInput(rightDirection, movementVector.X);
			}
			break;
		case false:
			AddMovementInput(GetActorForwardVector(), movementVector.Y);
			AddMovementInput(GetActorRightVector(), movementVector.X);
			break;
		default:
			break;
	}
}

void APlayerCharacter::Look(const FInputActionValue& actionValue)
{
	if (!IsValid(Controller)) return;

	FVector2D lookVector = actionValue.Get<FVector2D>();

	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(lookVector.Y);
}

void APlayerCharacter::OrbitalCameraChange()
{
	if (!IsValid(Controller)) return;

	switch (bUseControllerRotationYaw)
	{
		case false:
			bUseControllerRotationYaw = true;
			break;
		case true:
			bUseControllerRotationYaw = false;
			break;
		default:
			break;
	}

}

void APlayerCharacter::ZoomInCameraChange()
{
	if (!IsValid(Controller)) return;

	springArm->TargetArmLength = FMath::Clamp(springArm->TargetArmLength-20.f, 100.f, 1000.f);
}

void APlayerCharacter::ZoomOutCameraChange()
{
	if (!IsValid(Controller)) return;

	springArm->TargetArmLength = FMath::Clamp(springArm->TargetArmLength+20.f, 100.f, 800.f);
}

void APlayerCharacter::Interact()
{
	FVector startLocation;
	FRotator rotation;

	GetActorEyesViewPoint(startLocation, rotation);

	FVector endLocation = startLocation + (rotation.Vector() * 300.f);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params);
	if (bHit)
	{
		AActor* hitActor = hitResult.GetActor();
		if (!IsValid(hitActor)) return;

		if (hitActor->GetClass()->ImplementsInterface(UInteractive::StaticClass()))
		{
			IInteractive::Execute_Interact(hitActor, this);
		}
	}

}

void APlayerCharacter::SelectMachineForLink()
{
	FVector startLocation;
	FRotator rotation;

	GetActorEyesViewPoint(startLocation, rotation);

	FVector endLocation = startLocation + (rotation.Vector() * 300.f);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (!GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params)){return;}

	AMachineBase* hitMachine = Cast<AMachineBase>(hitResult.GetActor());
	if (!IsValid(hitMachine)){return;}

	if (!IsValid(pendingLinkSource)){
		pendingLinkSource = hitMachine;
		return;
	}

	if (pendingLinkSource != hitMachine){
		AConveyorBelt::ConnectMachines(this, defaultConveyorBeltClass, pendingLinkSource, hitMachine);
	}
	pendingLinkSource = nullptr;
}

void APlayerCharacter::CancelLink()
{
	pendingLinkSource = nullptr;
}

void APlayerCharacter::InventoryMenu()
{
	if (!IsValid(Controller)) return;

	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (!IsValid(playerController)) return;

	APlayerHUD* hud = Cast<APlayerHUD>(playerController->GetHUD());
	if (!IsValid(hud)) return;

	hud->OpenCloseInventory();

	if (!IsValid(inventoryComponent)) return;

	inventoryComponent->PrintInventory();
}

void APlayerCharacter::BuildMenu()
{
	if (!IsValid(Controller)) return;
	if (constructionPart != nullptr) constructionPart = nullptr;

	ChangeToBuildMappingContext();

	OpenCloseBuildMenu();
}

void APlayerCharacter::StartSprint()
{
	if (!IsValid(Controller)) return;
	bWantsToSprint = true;
}

void APlayerCharacter::StopSprint()
{
	bWantsToSprint = false;
}

void APlayerCharacter::RotateLeftStructure()
{
	if (!IsValid(Controller)) return;

	constructionComponent->RotateLeftStructure();
}

void APlayerCharacter::RotateRightStructure()
{
	if (!IsValid(Controller)) return;

	constructionComponent->RotateRightStructure();
}

void APlayerCharacter::PlaceStructure()
{
	if (!IsValid(Controller)) return;

	constructionComponent->PlaceStructure();
	constructionComponent->CreateStructure(constructionPart);
}

void APlayerCharacter::EndBuild()
{
	if (!IsValid(Controller)) return;

	OpenCloseBuildMenu();

	constructionComponent->EndBuild();

	if (constructionPart != nullptr)
	{
		constructionPart = nullptr;
	}
}
