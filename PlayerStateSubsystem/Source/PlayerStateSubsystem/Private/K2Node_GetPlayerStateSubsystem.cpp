#include "K2Node_GetPlayerStateSubsystem.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "PlayerStateSubsystemBlueprintLibrary.h"
#include "PlayerState_WithSubsystem.h"

void UK2Node_GetPlayerStateSubsystem::AllocateDefaultPins()
{
    // Create input pin for PlayerState
    CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, TEXT(""), APlayerState::StaticClass(), TEXT("PlayerState"));

    // Create input pin for Class if CustomClass is not set
    if (!CustomClass)
    {
        CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Class, USubsystem::StaticClass(), TEXT("Class"));
    }

    // Call base class implementation
    Super::AllocateDefaultPins();
}

void UK2Node_GetPlayerStateSubsystem::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    // Ensure node is expanded correctly
    UK2Node::ExpandNode(CompilerContext, SourceGraph);

    // Validate pins and return early if validation fails
    if (!ValidateClassPin(CompilerContext))
    {
        BreakAllNodeLinks();
        return;
    }

    // Determine function name and return early if invalid
    FName Get_FunctionName = DetermineFunctionName(CompilerContext);
    if (Get_FunctionName.IsNone())
    {
        BreakAllNodeLinks();
        return;
    }

    // Create and connect call node
    CreateAndConnectCallNode(CompilerContext, SourceGraph, Get_FunctionName);

    // Break all node links after expansion
    BreakAllNodeLinks();
}

bool UK2Node_GetPlayerStateSubsystem::ValidateClassPin(FKismetCompilerContext& CompilerContext)
{
    UEdGraphPin* ClassPin = GetClassPin();
    UClass* Class = (ClassPin != nullptr) ? Cast<UClass>(ClassPin->DefaultObject) : nullptr;

    if (ClassPin && (ClassPin->LinkedTo.Num() == 0) && !Class)
    {
        CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node @@ must have a class specified.").ToString(), this);
        BreakAllNodeLinks();
        return false;
    }

    return true;
}

FName UK2Node_GetPlayerStateSubsystem::DetermineFunctionName(FKismetCompilerContext& CompilerContext)
{
    if (CustomClass && CustomClass->IsChildOf<UPlayerStateSubsystem>())
    {
        return GET_FUNCTION_NAME_CHECKED(UPlayerStateSubsystemBlueprintLibrary, GetPlayerStateSubsystem);
    }
    
    CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node @@ must have a valid PlayerStateSubsystem class specified.").ToString(), this);
    BreakAllNodeLinks();
    return FName();
}

void UK2Node_GetPlayerStateSubsystem::CreateAndConnectCallNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph, FName Get_FunctionName)
{
    UK2Node_CallFunction* CallGetNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
    CallGetNode->FunctionReference.SetExternalMember(Get_FunctionName, UPlayerStateSubsystemBlueprintLibrary::StaticClass());
    CallGetNode->AllocateDefaultPins();

    UEdGraphPin* CallPlayerStatePin = CallGetNode->FindPin(FName("ContextObject"));
    UEdGraphPin* CallClassTypePin = CallGetNode->FindPin(TEXT("Class"));
    UEdGraphPin* CallResult = CallGetNode->GetReturnValuePin();

    if (!CallPlayerStatePin || !CallClassTypePin || !CallResult)
    {
        CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node @@ failed to create necessary pins.").ToString(), this);
        BreakAllNodeLinks();
        return;
    }

    MoveClassPinToIntermediate(CompilerContext, CallClassTypePin);
    MovePlayerStatePinToIntermediate(CompilerContext, CallPlayerStatePin);
    MoveResultPinToIntermediate(CompilerContext, CallResult);
}

void UK2Node_GetPlayerStateSubsystem::MoveClassPinToIntermediate(FKismetCompilerContext& CompilerContext, UEdGraphPin* CallClassTypePin)
{
    UEdGraphPin* ClassPin = GetClassPin();
    UClass* Class = (ClassPin != nullptr) ? Cast<UClass>(ClassPin->DefaultObject) : nullptr;

    if (ClassPin && ClassPin->LinkedTo.Num() > 0)
    {
        CompilerContext.MovePinLinksToIntermediate(*ClassPin, *CallClassTypePin);
    }
    else if (Class)
    {
        CallClassTypePin->DefaultObject = Class;
    }
    else if (CustomClass)
    {
        CallClassTypePin->DefaultObject = CustomClass;
    }
    else
    {
        CompilerContext.MessageLog.Error(*NSLOCTEXT("K2Node", "GetSubsystem_Error", "Node @@ must have a valid class connection or default value.").ToString(), this);
        BreakAllNodeLinks();
    }
}

void UK2Node_GetPlayerStateSubsystem::MovePlayerStatePinToIntermediate(FKismetCompilerContext& CompilerContext, UEdGraphPin* CallPlayerStatePin)
{
    UEdGraphPin* PlayerStatePin = GetPlayerStatePin();
    if (PlayerStatePin)
    {
        CompilerContext.MovePinLinksToIntermediate(*PlayerStatePin, *CallPlayerStatePin);
    }
}

void UK2Node_GetPlayerStateSubsystem::MoveResultPinToIntermediate(FKismetCompilerContext& CompilerContext, UEdGraphPin* CallResult)
{
    UEdGraphPin* ResultPin = GetResultPin();
    CallResult->PinType = ResultPin->PinType;
    CompilerContext.MovePinLinksToIntermediate(*ResultPin, *CallResult);
}

FText UK2Node_GetPlayerStateSubsystem::GetTooltipText() const
{
    if (CustomClass)
    {
        return FText::FormatNamed(NSLOCTEXT("K2Node", "GetSubsystem_TooltipFormat", "Get {ClassName} (PlayerStateSubsystem)\n\n{ClassTooltip}"),
                    TEXT("ClassName"), CustomClass->GetDisplayNameText(),
                    TEXT("ClassTooltip"), CustomClass->GetToolTipText(/*bShortTooltip=*/ true));
    }
    return NSLOCTEXT("K2Node", "GetPlayerStateSubsystem_InvalidSubsystemTypeTooltip", "Invalid Subsystem Type");
}

void UK2Node_GetPlayerStateSubsystem::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
    static TArray<UClass*> Subclasses;
    Subclasses.Reset();
    GetDerivedClasses(UPlayerStateSubsystem::StaticClass(), Subclasses);

    auto CustomizeCallback = [](UEdGraphNode* Node, bool bIsTemplateNode, UClass* Subclass)
    {
        auto TypedNode = CastChecked<UK2Node_GetPlayerStateSubsystem>(Node);
        TypedNode->Initialize(Subclass);
    };

    UClass* ActionKey = GetClass();
    if (ActionRegistrar.IsOpenForRegistration(ActionKey))
    {
        for (UClass* Iter : Subclasses)
        {
            if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(Iter, true))
            {
                continue;
            }

            UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(ActionKey);
            check(Spawner);

            Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeCallback, Iter);
            ActionRegistrar.AddBlueprintAction(ActionKey, Spawner);
        }
    }
}

FText UK2Node_GetPlayerStateSubsystem::GetMenuCategory() const
{
    return NSLOCTEXT("K2Node", "K2Node_GetPlayerStateSubsystem", "PlayerStateSubsystem");
}

UEdGraphPin* UK2Node_GetPlayerStateSubsystem::GetPlayerStatePin() const
{
    UEdGraphPin* Pin = FindPin(TEXT("PlayerState"));
    check(Pin == nullptr || Pin->Direction == EGPD_Input);
    return Pin;
}