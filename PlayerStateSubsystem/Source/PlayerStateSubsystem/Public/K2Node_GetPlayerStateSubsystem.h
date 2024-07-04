// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_GetSubsystem.h"
#include "K2Node_GetPlayerStateSubsystem.generated.h"

/**
 * K2Node to get a PlayerStateSubsystem
 */
UCLASS()
class PLAYERSTATESUBSYSTEM_API UK2Node_GetPlayerStateSubsystem : public UK2Node_GetSubsystem
{
	GENERATED_BODY()

public:
	//~ Begin UEdGraphNode interface.
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FText GetTooltipText() const override;
	//~ End UEdGraphNode interface.

	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;

	/** Get the world context input pin, can return NULL */
	UEdGraphPin* GetPlayerStatePin() const;

private:
	/**
	 * Ensure that the class pin is valid, connected, and that a target class can be determined
	 * @return true if the class pin is valid, false otherwise
	 */
	bool ValidateClassPin(FKismetCompilerContext& CompilerContext);

	/** Determine the name of the function to call on the blueprint library
	 * @return The name of the function to call, or NAME_None if the function could not be determined
	 */
	FName DetermineFunctionName(FKismetCompilerContext& CompilerContext);

	/**
	 * @param SourceGraph Blueprint graph this node is in
	 * @param Get_FunctionName Name of the function to call on the blueprint library
	 */
	void CreateAndConnectCallNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph, FName Get_FunctionName);

	/* Move this node's pins to the call node, which calls the Blueprint Library getter function */
	void MoveClassPinToIntermediate(FKismetCompilerContext& CompilerContext, UEdGraphPin* CallClassTypePin);
	void MovePlayerStatePinToIntermediate(FKismetCompilerContext& CompilerContext, UEdGraphPin* CallPlayerStatePin);
	void MoveResultPinToIntermediate(FKismetCompilerContext& CompilerContext, UEdGraphPin* CallResult);
};