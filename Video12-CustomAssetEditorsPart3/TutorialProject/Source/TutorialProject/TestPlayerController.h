#pragma once

#include "CoreMinimal.h"
#include "TestPlayerController.generated.h"

UCLASS()
class ATestPlayerController : public APlayerController {
    GENERATED_BODY()
public:
    ATestPlayerController();
    virtual ~ATestPlayerController();

    virtual void BeginPlay() override;

protected:
    UPROPERTY()
    class UDialogAsset* _dialogAsset = nullptr;

    UPROPERTY()
    class UDialogPlayer* _dialogPlayer = nullptr;
    
    UPROPERTY()
    class UNewQuestOverlayController* _newQuestOverlay = nullptr;
};
