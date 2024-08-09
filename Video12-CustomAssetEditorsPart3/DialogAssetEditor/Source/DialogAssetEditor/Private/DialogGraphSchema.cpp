#include "DialogGraphSchema.h"
#include "DialogGraphNode.h"
#include "DialogStartGraphNode.h"
#include "DialogEndGraphNode.h"
#include "DialogNodeInfo.h"

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const {
    TSharedPtr<FNewNodeAction> newDialogNodeAction(
        new FNewNodeAction(
            UDialogGraphNode::StaticClass(),
            FText::FromString(TEXT("Nodes")),
            FText::FromString(TEXT("New Dialog Node")),
            FText::FromString(TEXT("Makes a new dialog node")),
            0
        )
    );

    TSharedPtr<FNewNodeAction> newEndNodeAction(
        new FNewNodeAction(
            UDialogEndGraphNode::StaticClass(),
            FText::FromString(TEXT("Nodes")),
            FText::FromString(TEXT("New End Node")),
            FText::FromString(TEXT("Makes a new end node")),
            0
        )
    );

    contextMenuBuilder.AddAction(newDialogNodeAction);
    contextMenuBuilder.AddAction(newEndNodeAction);
}

const FPinConnectionResponse UDialogGraphSchema::CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const {
    if (a == nullptr || b == nullptr) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
    }

    if (a->Direction == b->Direction) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

void UDialogGraphSchema::CreateDefaultNodesForGraph(UEdGraph& graph) const {
    UDialogStartGraphNode* startNode = NewObject<UDialogStartGraphNode>(&graph);
    startNode->CreateNewGuid();
    startNode->NodePosX = 0;
    startNode->NodePosY = 0;

    startNode->CreateDialogPin(EEdGraphPinDirection::EGPD_Output, FName(TEXT("Start")));
    
    graph.AddNode(startNode, true, true);
    graph.Modify();
}

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode) {
    UDialogGraphNodeBase* result = NewObject<UDialogGraphNodeBase>(parentGraph, _classTemplate);
    result->CreateNewGuid();
    result->NodePosX = location.X;
    result->NodePosY = location.Y;
    result->InitNodeInfo(result);

    UEdGraphPin* inputPin = result->CreateDefaultInputPin();
    result->CreateDefaultOutputPins();

    if (fromPin != nullptr) {
        result->GetSchema()->TryCreateConnection(fromPin, inputPin);
    }

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
