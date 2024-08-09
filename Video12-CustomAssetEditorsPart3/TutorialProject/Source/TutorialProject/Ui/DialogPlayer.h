#pragma once

#include "CoreMinimal.h"
#include <functional>
#include "DialogEndNodeInfo.h"
#include "DialogPlayer.generated.h"

UCLASS()
class UDialogPlayer : public UObject {
    GENERATED_BODY()

public:
    void PlayDialog(class UDialogAsset* dialogAsset, APlayerController* playerController, std::function<void(EDialogNodeAction type, FString data)> onDialogEnded);
    void ChooseOptionAtIndex(int index);

private:
    UPROPERTY()
    class UDialogAsset* _playingAsset = nullptr;

    UPROPERTY()
    class UDialogRuntimeNode* _currentNode = nullptr;

    UPROPERTY()
    class UQuestDialogUiController* _dialogUi = nullptr;

    std::function<void(EDialogNodeAction type, FString data)> _onDialogEnded;
};
