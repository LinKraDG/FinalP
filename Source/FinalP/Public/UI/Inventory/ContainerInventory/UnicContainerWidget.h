// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ItemData.h"
#include "UnicContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UUnicContainerWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	FItemData* itemData;
	
	UPROPERTY(EditAnywhere)
	int ID;

	UPROPERTY(EditAnywhere)
	int quantity;

	UPROPERTY(EditAnywhere)
	bool selected;
	
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> frame{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> item{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> selectedBorder{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> quantityText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> itemButton{};

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* imageItemFrame;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* imageFrame;

	//Buttons functions
	UFUNCTION()
	void OnItemButtonPressed();

public:
	UFUNCTION()
	UBorder* GetFrame();

	UFUNCTION()
	UBorder* GetItem();

	UFUNCTION()
	bool GetSelected();

	UFUNCTION()
	int GetItemID();
	
	UFUNCTION()
	int GetQuantity();
	
	UFUNCTION(BlueprintCallable)
	void SetNoneImage();
	
	UFUNCTION(BlueprintCallable)
	void SetFrameImage();

	UFUNCTION()
	void SetData(int slot_ID, FItemData& data, int itemQuantity);

	UFUNCTION()
	void SetEmptyData(int slot_ID);
};
