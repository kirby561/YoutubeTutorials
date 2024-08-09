#include "DialogPlayer.h"
#include "DialogAsset.h"
#include "DialogEndNodeInfo.h"
#include "QuestDialogUiController.h"
#include "QuestResponseButtonController.h"

#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

DEFINE_LOG_CATEGORY_STATIC(DialogPlayerSub, Log, All);

void UDialogPlayer::PlayDialog(UDialogAsset* dialogAsset, APlayerController* playerController, std::function<void(EDialogNodeAction type, FString data)> onDialogEnded) {
    _onDialogEnded = onDialogEnded;
    UDialogRuntimeGraph* graph = dialogAsset->Graph;

    // Get the start node first
    for (UDialogRuntimeNode* node : graph->Nodes) {
        if (node->NodeType == EDialogNodeType::StartNode) {
            _currentNode = node;
            break;
        }
    }
    
    if (_currentNode == nullptr) {
        UE_LOG(DialogPlayerSub, Error, TEXT("No start node in dialog."));
        return;
    }

    // Create and display the dialog UI
    _dialogUi = UQuestDialogUiController::CreateInstance(playerController);
    _dialogUi->AddToViewport();

    // Play the first index to get started
    ChooseOptionAtIndex(0);
}

void UDialogPlayer::ChooseOptionAtIndex(int index) {
    // Move to the given index if we can
    if (index >= _currentNode->OutputPins.Num() || index < 0) {
        UE_LOG(DialogPlayerSub, Error, TEXT("Invalid response option at index %d"), index);
        return;
    }

    UDialogRuntimePin* outputPin = _currentNode->OutputPins[index];
    if (outputPin->Connection != nullptr) {
        _currentNode = outputPin->Connection->Parent;
    } else {
        // No connection so we'll assume it's an end node
        _currentNode = nullptr;
    }

    if (_currentNode != nullptr && _currentNode->NodeType == EDialogNodeType::DialogNode) {
        UDialogNodeInfo* nodeInfo = Cast<UDialogNodeInfo>(_currentNode->NodeInfo);
        _dialogUi->DialogText->SetText(nodeInfo->DialogText);
        
        _dialogUi->ResponsesBox->ClearChildren();
        int optionIndex = 0;
        for (FText response : nodeInfo->DialogResponses) {
            UQuestResponseButtonController* button = UQuestResponseButtonController::CreateInstance(_dialogUi->GetOwningPlayer());
            button->SetClickHandler(optionIndex, [this] (int optionIndex) {
                ChooseOptionAtIndex(optionIndex);
            });
            button->ResponseButtonText->SetText(response);
            UHorizontalBoxSlot* slot = _dialogUi->ResponsesBox->AddChildToHorizontalBox(button);
            slot->SetPadding(FMargin(10));

            optionIndex++;
        }
    } else if (_currentNode == nullptr || _currentNode->NodeType == EDialogNodeType::EndNode) {
        APlayerController* playerController = _dialogUi->GetOwningPlayer();
        _dialogUi->RemoveFromParent();
        _dialogUi = nullptr;

        EDialogNodeAction action = EDialogNodeAction::None;
        FString actionData = TEXT("");
        if (_currentNode != nullptr) {
            UDialogEndNodeInfo* endNodeInfo = Cast<UDialogEndNodeInfo>(_currentNode->NodeInfo);
            action = endNodeInfo->Action;
            actionData = endNodeInfo->ActionData;
        }

        _onDialogEnded(action, actionData);
    }
}
