// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ConstructionData.h"
#include "UnicStructure.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UUnicStructure : public UUserWidget
{
	GENERATED_BODY()

private:
	FConstructionData constructionData;

public:
	
	UPROPERTY(EditAnywhere)
	int ID;
	
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class APlayerHUD> hud{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> frame{};
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* imageStructureFrame;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> nameText{};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> selectionButton{};

	//Buttons functions
	UFUNCTION()
	void OnStructureButtonPressed();

public:
	UFUNCTION()
	UBorder* GetFrame();

	UFUNCTION()
	void SetData(FConstructionData data);
};
