#include "TestPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(TestPlayerControllerSub, Log, All);

ATestPlayerController::ATestPlayerController() {

}

ATestPlayerController::~ATestPlayerController() {
}

void ATestPlayerController::BeginPlay() {
    Super::BeginPlay();

    UE_LOG(TestPlayerControllerSub, Display, TEXT("BeginPlay"));
}
