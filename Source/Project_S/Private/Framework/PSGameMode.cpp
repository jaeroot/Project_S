// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PSGameMode.h"
#include "Character/PSCharacter.h"
#include "Character/PSPlayerController.h"


APSGameMode::APSGameMode()
{
	DefaultPawnClass = APSCharacter::StaticClass();
	PlayerControllerClass = APSPlayerController::StaticClass();
}