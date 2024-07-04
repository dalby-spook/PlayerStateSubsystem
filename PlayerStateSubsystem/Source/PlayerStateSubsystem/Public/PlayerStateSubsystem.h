// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "PlayerStateSubsystem.generated.h"

class APlayerState_WithSubsystem;
/**
 * 
 */
UCLASS(Abstract, Within = PlayerState_WithSubsystem)
class PLAYERSTATESUBSYSTEM_API UPlayerStateSubsystem : public USubsystem
{
	GENERATED_BODY()
	
public:
	UPlayerStateSubsystem();

	template<typename PlayerStateType = APlayerState_WithSubsystem>
	PlayerStateType* GetPlayerState() const
	{
		return Cast<PlayerStateType>(GetOuter());
	}

	template<typename PlayerStateType = APlayerState_WithSubsystem>
	PlayerStateType* GetPlayerStateChecked() const
	{
		return CastChecked<PlayerStateType>(GetOuter());
	}
};
