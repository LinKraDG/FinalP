// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "AsyncTreeDifferences.h"
#include "UI/PlayerHUD.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/ConstructionComponent.h"
#include "Character/Components/InventoryComponent.h"
#include "Construction/ConstructionPart.h"
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

	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 400.f;
	springArm->bUsePawnControlRotation = true;
	
	camera->SetupAttachment(springArm);
	camera->bUsePawnControlRotation = false;
	
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
/*void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

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
	EDefaultInputComponent->BindAction(buildAction, ETriggerEvent::Triggered, this, &APlayerCharacter::BuildMenu);
	EDefaultInputComponent->BindAction(interactAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
	EDefaultInputComponent->BindAction(inventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InventoryMenu);
	//Build actions
	EDefaultInputComponent->BindAction(rotateStructureAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RotateStructure);
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

	OpenCloseBuildMenu();
	
	CreateStructure();
}

void APlayerCharacter::CreateStructure()
{
	actBuilding = GetWorld()->SpawnActor<AConstructionPart>(constructionPart, FVector(0,0,0), lastRotator);
	if (!IsValid(actBuilding)) return;

	actBuilding->SetTransparentMaterial();
}


void APlayerCharacter::Move(const FInputActionValue& actionValue)
{
	if (!IsValid(Controller)) return;
	
	FVector2D movementVector = actionValue.Get<FVector2D>();

	switch (bUseControllerRotationYaw)
	{
		case true:
			const FRotator rotation = Controller->GetControlRotation();
			const FRotator yawRotation = FRotator(0,rotation.Yaw,0);

			const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
			const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(forwardDirection, movementVector.Y);
			AddMovementInput(rightDirection, movementVector.X);
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
	//UE_LOG(LogTemp, Warning, TEXT("El valor es: %f"), cameraAxis);
	
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
	if (constructionPart != NULL)
	{
		constructionPart = NULL;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Hora de construir"));
	APlayerController* playerController = Cast<APlayerController>(Controller);
	
	if (!IsValid(playerController)) return;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (!IsValid(subsystem)) return;

	subsystem->AddMappingContext(buildMappingContext, 1);
	UE_LOG(LogTemp, Warning, TEXT("Mapeo añadido"));
	OpenCloseBuildMenu();
}

void APlayerCharacter::MoveStructure()
{
	if (!IsValid(Controller)) return;
	if (!IsValid(actBuilding)) return;
	
	//-------------------------------------------------------------------------
	
	FVector startLocation;
	FRotator rotation;

	GetActorEyesViewPoint(startLocation, rotation);

	FVector endLocation = startLocation + (rotation.Vector() * 600.f);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params);
	FVector place;
	switch (bHit)
	{
		case true:
			
			place = {round(hitResult.Location.X/100)*100,round(hitResult.Location.Y/100)*100,round(hitResult.Location.Z/100)*100};
			actBuilding->SetActorLocation(place);
			break;
		case false:
			place = {round(endLocation.X/100)*100,round(endLocation.Y/100)*100,round(endLocation.Z/100)*100};
			actBuilding->SetActorLocation(FVector(hitResult.Location.X,hitResult.Location.Y,hitResult.Location.Z));
			break;
	}

	///////////////////////
}

void APlayerCharacter::RotateStructure()
{
	if (!IsValid(Controller)) return;
	
	//-------------------------------------------------------------------------
	
}

void APlayerCharacter::PlaceStructure()
{
	if (!IsValid(Controller)) return;
	if (!IsValid(actBuilding)) return;
	actBuilding->ChangeMaterial();
	CreateStructure();
}

void APlayerCharacter::EndBuild()
{
	if (!IsValid(Controller)) return;

	UE_LOG(LogTemp, Warning, TEXT("Hora de terminar de construir"));
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (!IsValid(playerController)) return;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (!IsValid(subsystem)) return;

	subsystem->RemoveMappingContext(buildMappingContext);
	UE_LOG(LogTemp, Warning, TEXT("Mapeo eliminado"));
	if (constructionPart != NULL)
	{
		constructionPart = NULL;
	}
	OpenCloseBuildMenu();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(actBuilding)) return;
	MoveStructure();
	
}
