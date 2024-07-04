// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "PlayerStateSubsystemBlueprintLibrary.generated.h"

class APlayerState_WithSubsystem;
class UPlayerStateSubsystem;
/**
 * 
 */
UCLASS()
class PLAYERSTATESUBSYSTEM_API UPlayerStateSubsystemBlueprintLibrary : public UAutomationBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Player State Subsystems", meta = (BlueprintInternalUseOnly = "true"))
	static UPlayerStateSubsystem* GetPlayerStateSubsystem(UObject* ContextObject, TSubclassOf<UPlayerStateSubsystem> Class);
private:
	static UWorld* GetWorldFrom(UObject* ContextObject);

};
