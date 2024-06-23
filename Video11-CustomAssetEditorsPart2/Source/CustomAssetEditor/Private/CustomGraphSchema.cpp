#include "CustomGraphSchema.h"
#include "CustomGraphNode.h"

void UCustomGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const {
    TSharedPtr<FNewNodeAction> newNodeAction(
        new FNewNodeAction(
            FText::FromString(TEXT("Nodes")), 
            FText::FromString(TEXT("New node")), 
            FText::FromString(TEXT("Makes a new node")), 
            0
        )
    );

    contextMenuBuilder.AddAction(newNodeAction);
}

const FPinConnectionResponse UCustomGraphSchema::CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const {
    // Neither can be nullptr
    if (a == nullptr || b == nullptr) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
    }

    // Make sure the directions are different (no inputs to inputs or outputs to outputs)
    if (a->Direction == b->Direction) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
    }

    // Else allow it but break all connections
    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode) {
    UCustomGraphNode* result = NewObject<UCustomGraphNode>(parentGraph);
    result->CreateNewGuid();
    result->NodePosX = location.X;
    result->NodePosY = location.Y;

    UEdGraphPin* inputPin = result->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, TEXT("SomeInput"));
    result->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("Output1"));
    result->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("Output2"));

    // If the from pin is not null, connect them
    if (fromPin != nullptr) {
        result->GetSchema()->TryCreateConnection(fromPin, inputPin);
    }

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
