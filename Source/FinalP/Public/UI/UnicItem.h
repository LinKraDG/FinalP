// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnicItem.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UUnicItem : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int ID;
	
protected:
	
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class APlayerHUD> hud{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> frame{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> item{};
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UVerticalBox> options{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> quantity;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> itemButton{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> dropButton{};

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* imageItemFrame;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* imageFrame;

	//Buttons functions
	UFUNCTION()
	void OnItemButtonPressed();

	UFUNCTION()
	void OnDropButtonPressed();

	//UFUNCTION()
	//void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:

	UFUNCTION()
	UBorder* GetFrame();

	UFUNCTION()
	UBorder* GetItem();

	UFUNCTION()
	UTextBlock* GetQuantityText();
	
	UFUNCTION()
	UVerticalBox* GetOptions();

	UFUNCTION(BlueprintCallable)
	void SetNoneImage();
	
	UFUNCTION(BlueprintCallable)
	void SetFrameImage();
	
};
