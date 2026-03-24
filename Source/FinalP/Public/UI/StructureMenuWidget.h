// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructureMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALP_API UStructureMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class AConstructionPart> selectedStructure{};
};
