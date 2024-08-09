#include "TestPlayerController.h"
#include "DialogAsset.h"
#include "DialogEndNodeInfo.h"
#include "Ui/NewQuestOverlayController.h"
#include "Ui/DialogPlayer.h"

#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(TestPlayerControllerSub, Log, All);

ATestPlayerController::ATestPlayerController() {
    static ConstructorHelpers::FObjectFinder<UDialogAsset> dialogAssetFinder(TEXT("/Game/TheBeginningQuestDialog"));
    if (dialogAssetFinder.Succeeded()) {
        _dialogAsset = dialogAssetFinder.Object;
    } else {
        UE_LOG(TestPlayerControllerSub, Error, TEXT("Could not load content"));
    }
}

ATestPlayerController::~ATestPlayerController() {
}

void ATestPlayerController::BeginPlay() {
    Super::BeginPlay();

    // Play the dialog
    _dialogPlayer = NewObject<UDialogPlayer>(this);
    _dialogPlayer->PlayDialog(_dialogAsset, this, [this] (EDialogNodeAction action, FString actionData) {
        if (action == EDialogNodeAction::StartQuest) {
            if (_newQuestOverlay != nullptr) {
                _newQuestOverlay->RemoveFromParent();
            }
            _newQuestOverlay = UNewQuestOverlayController::CreateInstance(this);
            _newQuestOverlay->QuestNameText->SetText(FText::FromString(actionData));
            _newQuestOverlay->AddToViewport();
        }
    });
}
