// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class FINALP_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AConstructionPart> constructionPart{};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AActor> interactiveItem{};

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<class UInventoryComponent> inventoryComponent{};

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<class UConstructionComponent> constructionComponent{};
	
	UFUNCTION()
	void OpenCloseBuildMenu();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	//Components------------------------------------
	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<class USpringArmComponent> springArm{};

	UPROPERTY(EditAnywhere, Category = Components)
	TObjectPtr<class UCameraComponent> camera{};
	
	//Default Inputs---------------------------------------
	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputMappingContext> defaultMappingContext{};

	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> moveAction{};
	
	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> lookAction{};
	
	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> orbitalCameraAction{};

	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> jumpAction{};

	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> interactAction{};

	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> inventoryAction{};

	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> zoomInCameraAction{};

	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> zoomOutCameraAction{};
	
	UPROPERTY(EditAnywhere, Category = DefaultInput)
	TObjectPtr<class UInputAction> buildAction{};


	//Build inputs----------------------------------
	UPROPERTY(EditAnywhere, Category = BuildInput)
	TObjectPtr<class UInputMappingContext> buildMappingContext{};

	UPROPERTY(EditAnywhere, Category = BuildInput)
	TObjectPtr<class UInputAction> rotateLeftStructureAction{};

	UPROPERTY(EditAnywhere, Category = BuildInput)
	TObjectPtr<class UInputAction> rotateRightStructureAction{};
	
	UPROPERTY(EditAnywhere, Category = BuildInput)
	TObjectPtr<class UInputAction> placeStructureAction{};

	UPROPERTY(EditAnywhere, Category = BuildInput)
	TObjectPtr<class UInputAction> endBuildAction{};
	
	//Default Input Functions------------------------
	void Move(const FInputActionValue& actionValue);
	void Look(const FInputActionValue& actionValue);
	void OrbitalCameraChange();
	void ZoomInCameraChange();
	void ZoomOutCameraChange();
	void Interact();
	void InventoryMenu();
	void BuildMenu();

	
	//Build Input Functions------------------------
	void RotateLeftStructure();
	void RotateRightStructure();
	void PlaceStructure();
	void EndBuild();

private:
	
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UInventoryComponent* GetInventory();

	UFUNCTION()
	UConstructionComponent* GetConstruction();

	UFUNCTION()
	void SetConstructionMode(TSubclassOf<AConstructionPart> part);

	UFUNCTION()
	void ChangeToBuildMappingContext();

	UFUNCTION()
	void ChangeToDefaultMappingContext();
	
};
