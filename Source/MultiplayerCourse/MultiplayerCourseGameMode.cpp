// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerCourseGameMode.h"
#include "MultiplayerCourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerCourseGameMode::AMultiplayerCourseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMultiplayerCourseGameMode::HostLANGame()
{
	GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");

}

void AMultiplayerCourseGameMode::JoinLANGame()
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (PC) 
	{
	PC->ClientTravel("192.168.51.162", ETravelType::TRAVEL_Absolute);
	}
}
