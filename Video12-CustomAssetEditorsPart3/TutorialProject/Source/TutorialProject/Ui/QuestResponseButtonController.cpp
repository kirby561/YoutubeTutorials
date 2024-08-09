#include "QuestResponseButtonController.h"

// ---------- Generated Includes Section ---------- //
//             (Don't modify manually)              //
#include "Components/Button.h"
#include "Components/TextBlock.h"
// ---------- End Generated Includes Section ---------- //

UQuestResponseButtonController::UQuestResponseButtonController(const FObjectInitializer& objectInitializer) : UUserWidget(objectInitializer) {

}

void UQuestResponseButtonController::SetClickHandler(int index, std::function<void(int)> onClickHandler) { 
    _index = index;
    _onClickHandler = onClickHandler;

    ResponseButton->OnClicked.AddUniqueDynamic(this, &UQuestResponseButtonController::OnButtonClicked);
}

void UQuestResponseButtonController::OnButtonClicked() {
    if (_onClickHandler) {
        _onClickHandler(_index);
    }
}

// ---------- Generated Methods Section ---------- //
//             (Don't modify manually)             //
UQuestResponseButtonController* UQuestResponseButtonController::CreateInstance(APlayerController* playerController) {
    UQuestResponseButtonLoader* loader = NewObject<UQuestResponseButtonLoader>(playerController);
    return Cast<UQuestResponseButtonController>(CreateWidget(playerController, loader->WidgetTemplate));
}

UQuestResponseButtonLoader::UQuestResponseButtonLoader() {
    static ConstructorHelpers::FClassFinder<UUserWidget> widgetTemplateFinder(*WidgetPath);
    WidgetTemplate = widgetTemplateFinder.Class;
}
// ---------- End Generated Methods Section ---------- //
