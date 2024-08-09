#include "DialogEndGraphNode.h"

FText UDialogEndGraphNode::GetNodeTitle(ENodeTitleType::Type titalType) const { 
    if (_nodeInfo != nullptr && _nodeInfo->Action != EDialogNodeAction::None) {
        FString result = UEnum::GetDisplayValueAsText(_nodeInfo->Action).ToString();
        if (!_nodeInfo->ActionData.IsEmpty()) {
            FString actionData = _nodeInfo->ActionData;
            if (actionData.Len() > 15) {
                actionData = actionData.Left(15) + TEXT("...");
            }
            result += TEXT(" - ") + actionData;
        }
        return FText::FromString(result);
    }

    return FText::FromString(TEXT("End"));
}

void UDialogEndGraphNode::GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const {
    FToolMenuSection& section = menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("End Node Actions")));

    UDialogEndGraphNode* node = (UDialogEndGraphNode*)this;
    section.AddMenuEntry(
        TEXT("DeleteEntry"),
        FText::FromString(TEXT("Delete Node")),
        FText::FromString(TEXT("Deletes the node")),
        FSlateIcon(TEXT("DialogAssetEditorStyle"), TEXT("DialogAssetEditor.NodeDeleteNodeIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [node] () {
                node->GetGraph()->RemoveNode(node);
            }
        ))
    );
}

UEdGraphPin* UDialogEndGraphNode::CreateDialogPin(EEdGraphPinDirection direction, FName name) {
    FName category = TEXT("Inputs");
    FName subcategory = TEXT("EndPin");

    UEdGraphPin* pin = CreatePin(
        direction,
        category,
        name
    );
    pin->PinType.PinSubCategory = subcategory;

    return pin;
}

UEdGraphPin* UDialogEndGraphNode::CreateDefaultInputPin() { 
    return CreateDialogPin(EEdGraphPinDirection::EGPD_Input, FName(TEXT("Finish")));
}
