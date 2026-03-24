// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGameMode.h"
#include "GameFramework/HUD.h"

AProjectGameMode::AProjectGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Game/Magtory/Character/Blueprint/BP_Player"));
	if (PlayerPawnClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClass(TEXT("/Game/Magtory/UI/BP_PlayerHUD"));
	if (PlayerHUDClass.Class != NULL)
	{
		HUDClass = PlayerHUDClass.Class;
	}
}
