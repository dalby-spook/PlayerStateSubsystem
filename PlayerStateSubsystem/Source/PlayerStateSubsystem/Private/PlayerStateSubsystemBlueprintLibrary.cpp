// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateSubsystemBlueprintLibrary.h"

#include "PlayerStateSubsystem.h"
#include "PlayerState_WithSubsystem.h"

UPlayerStateSubsystem* UPlayerStateSubsystemBlueprintLibrary::GetPlayerStateSubsystem(UObject* ContextObject, TSubclassOf<UPlayerStateSubsystem> Class)
{
	APlayerState* PlayerState;

	if (APlayerController* PlayerController = Cast<APlayerController>(ContextObject))
	{
		PlayerState = PlayerController->PlayerState;
	}
	else if(APawn* Pawn = Cast<APawn>(ContextObject))
	{
		PlayerState = Pawn->GetPlayerState();
	}
	else
	{
		PlayerState = Cast<APlayerState>(ContextObject);
	}

	if (PlayerState != nullptr)
	{
		APlayerState_WithSubsystem* PlayerState_WithSubsystem = Cast<APlayerState_WithSubsystem>(PlayerState);
		if (PlayerState_WithSubsystem)
		{
			return PlayerState_WithSubsystem->GetSubsystemBase(Class);
		}
	}

	return nullptr;
}


UWorld* UPlayerStateSubsystemBlueprintLibrary::GetWorldFrom(UObject* ContextObject)
{
	if (ContextObject)
	{
		return GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::ReturnNull);
	}
	return nullptr;
}
