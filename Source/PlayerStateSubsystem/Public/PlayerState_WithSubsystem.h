// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_WithSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERSTATESUBSYSTEM_API APlayerState_WithSubsystem : public APlayerState
{
	GENERATED_BODY()
	friend class UPlayerStateSubsystem;
public:
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;
	
	UPlayerStateSubsystem* GetSubsystemBase(TSubclassOf<UPlayerStateSubsystem> SubsystemClass) const
	{
		return SubsystemCollection.GetSubsystem<UPlayerStateSubsystem>(SubsystemClass);
	}

	template<typename TSubsystemClass>
	TSubsystemClass* GetSubsystem(TSubclassOf<UPlayerStateSubsystem> SubsystemClass) const
	{
		return SubsystemCollection.GetSubsystem<TSubsystemClass>(SubsystemClass);
	}

	template<typename TSubsystemClass>
	static FORCEINLINE TSubsystemClass* GetSubsystem(const APlayerState_WithSubsystem* PlayerState)
	{
		if (PlayerState)
		{
			return PlayerState->GetSubsystem<TSubsystemClass>();
		}
		return nullptr;
	}

	template<typename TSubsystemClass>
	const TArray<TSubsystemClass*> GetSubsystems() const
	{
		return SubsystemCollection.GetSubsystemArray<TSubsystemClass>(TSubsystemClass::StaticClass());
	}

private:
	FObjectSubsystemCollection<UPlayerStateSubsystem> SubsystemCollection;
};
