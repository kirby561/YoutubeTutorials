#include "NewQuestOverlayController.h"

// ---------- Generated Includes Section ---------- //
//             (Don't modify manually)              //
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
// ---------- End Generated Includes Section ---------- //

UNewQuestOverlayController::UNewQuestOverlayController(const FObjectInitializer& objectInitializer) : UUserWidget(objectInitializer) {

}

// ---------- Generated Methods Section ---------- //
//             (Don't modify manually)             //
UNewQuestOverlayController* UNewQuestOverlayController::CreateInstance(APlayerController* playerController) {
    UNewQuestOverlayLoader* loader = NewObject<UNewQuestOverlayLoader>(playerController);
    return Cast<UNewQuestOverlayController>(CreateWidget(playerController, loader->WidgetTemplate));
}

UNewQuestOverlayLoader::UNewQuestOverlayLoader() {
    static ConstructorHelpers::FClassFinder<UUserWidget> widgetTemplateFinder(*WidgetPath);
    WidgetTemplate = widgetTemplateFinder.Class;
}
// ---------- End Generated Methods Section ---------- //
