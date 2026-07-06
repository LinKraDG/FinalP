// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DatabaseSettings.generated.h"

class UDataTable;

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Game Databases"))
class FINALP_API UDatabaseSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Database")
	TSoftObjectPtr<UDataTable> itemTable;

	UPROPERTY(Config, EditAnywhere, Category = "Database")
	TSoftObjectPtr<UDataTable> recipeTable;

	UPROPERTY(Config, EditAnywhere, Category = "Database")
	TSoftObjectPtr<UDataTable> techTable;
};
