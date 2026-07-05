// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Pause/PauseMenuWidget.h"

#include "Character/PlayerCharacter.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	resumeButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::OnResumeButtonPressed);
	exitButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::OnExitButtonPressed);
}

void UPauseMenuWidget::OnResumeButtonPressed()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(player)) return;

	player->PauseGame();
}

void UPauseMenuWidget::OnExitButtonPressed()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
