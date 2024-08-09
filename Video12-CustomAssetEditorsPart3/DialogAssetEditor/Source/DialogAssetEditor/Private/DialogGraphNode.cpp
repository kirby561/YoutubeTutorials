#include "DialogGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"
#include "DialogNodeInfo.h"

FText UDialogGraphNode::GetNodeTitle(ENodeTitleType::Type titalType) const { 
    UDialogNodeInfo* nodeInfo = Cast<UDialogNodeInfo>(_nodeInfo);
    if (nodeInfo->Title.IsEmpty()) {
        FString dialogTextStr = nodeInfo->DialogText.ToString();
        if (dialogTextStr.Len() > 15) {
            dialogTextStr = dialogTextStr.Left(15) + TEXT("...");
        }
        return FText::FromString(dialogTextStr);
    }
    return nodeInfo->Title;
}

void UDialogGraphNode::GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const {
    FToolMenuSection& section = menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Dialog Node Actions")));

    UDialogGraphNode* node = (UDialogGraphNode*)this;
    section.AddMenuEntry(
        TEXT("AddPinEntry"),
        FText::FromString(TEXT("Add Response")),
        FText::FromString(TEXT("Creates a new response")),
        FSlateIcon(TEXT("DialogAssetEditorStyle"), TEXT("DialogAssetEditor.NodeAddPinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [node] () {
                node->GetDialogNodeInfo()->DialogResponses.Add(FText::FromString(TEXT("Response")));
                node->SyncPinsWithResponses();
                node->GetGraph()->NotifyGraphChanged();
                node->GetGraph()->Modify();
            }
        ))
    );
    
    section.AddMenuEntry(
        TEXT("DeletePinEntry"),
        FText::FromString(TEXT("Delete Response")),
        FText::FromString(TEXT("Deletes the last response")),
        FSlateIcon(TEXT("DialogAssetEditorStyle"), TEXT("DialogAssetEditor.NodeDeletePinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [node] () {
                UEdGraphPin* pin = node->GetPinAt(node->Pins.Num() - 1);
                if (pin->Direction != EEdGraphPinDirection::EGPD_Input) {
                    UDialogNodeInfo* info = node->GetDialogNodeInfo();
                    info->DialogResponses.RemoveAt(info->DialogResponses.Num() - 1);
                    node->SyncPinsWithResponses();

                    node->GetGraph()->NotifyGraphChanged();
                    node->GetGraph()->Modify();
                }
            }
        ))
    );

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

UEdGraphPin* UDialogGraphNode::CreateDialogPin(EEdGraphPinDirection direction, FName name) {
    FName category = (direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
    FName subcategory = TEXT("DialogPin");

    UEdGraphPin* pin = CreatePin(
        direction,
        category,
        name
    );
    pin->PinType.PinSubCategory = subcategory;

    return pin;
}

UEdGraphPin* UDialogGraphNode::CreateDefaultInputPin() { 
    return CreateDialogPin(EEdGraphPinDirection::EGPD_Input, TEXT("Display"));
}

void UDialogGraphNode::CreateDefaultOutputPins() {
    FString defaultResponse = TEXT("Continue");
    CreateDialogPin(EEdGraphPinDirection::EGPD_Output, FName(defaultResponse));
    GetDialogNodeInfo()->DialogResponses.Add(FText::FromString(defaultResponse));
}

void UDialogGraphNode::SyncPinsWithResponses() {
    // Sync the pins on the node with the dialog responses
    // We're going to assume the first pin is always the
    // input pin
    UDialogNodeInfo* nodeInfo = GetDialogNodeInfo();
    int numGraphNodePins = Pins.Num() - 1;
    int numInfoPins = nodeInfo->DialogResponses.Num();

    while (numGraphNodePins > numInfoPins) {
        RemovePinAt(numGraphNodePins - 1, EEdGraphPinDirection::EGPD_Output);
        numGraphNodePins--;
    }
    while (numInfoPins > numGraphNodePins) {
        CreateDialogPin(
            EEdGraphPinDirection::EGPD_Output,
            FName(nodeInfo->DialogResponses[numGraphNodePins].ToString())
        );
        numGraphNodePins++;
    }

    int index = 1;
    for (const FText& option : nodeInfo->DialogResponses) {
        GetPinAt(index)->PinName = FName(option.ToString());
        index++;
    }
}
