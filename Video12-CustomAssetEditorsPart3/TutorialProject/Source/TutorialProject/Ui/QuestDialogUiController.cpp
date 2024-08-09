#include "QuestDialogUiController.h"

// ---------- Generated Includes Section ---------- //
//             (Don't modify manually)              //
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
// ---------- End Generated Includes Section ---------- //

UQuestDialogUiController::UQuestDialogUiController(const FObjectInitializer& objectInitializer) : UUserWidget(objectInitializer) {

}

// ---------- Generated Methods Section ---------- //
//             (Don't modify manually)             //
UQuestDialogUiController* UQuestDialogUiController::CreateInstance(APlayerController* playerController) {
    UQuestDialogUiLoader* loader = NewObject<UQuestDialogUiLoader>(playerController);
    return Cast<UQuestDialogUiController>(CreateWidget(playerController, loader->WidgetTemplate));
}

UQuestDialogUiLoader::UQuestDialogUiLoader() {
    static ConstructorHelpers::FClassFinder<UUserWidget> widgetTemplateFinder(*WidgetPath);
    WidgetTemplate = widgetTemplateFinder.Class;
}
// ---------- End Generated Methods Section ---------- //
