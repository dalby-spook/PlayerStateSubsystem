// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_WithSubsystem.h"

void APlayerState_WithSubsystem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SubsystemCollection.Initialize(this);
}

void APlayerState_WithSubsystem::Destroyed()
{
	SubsystemCollection.Deinitialize();
	Super::Destroyed();
}
