#pragma once

#include "CoreMinimal.h"
#include "DialogNodeInfoBase.h"
#include "DialogEndNodeInfo.generated.h"

UENUM(BlueprintType)
enum class EDialogNodeAction : uint8 {
    None,
    StartQuest // ActionData is the QuestId
};

UCLASS(BlueprintType)
class DIALOGASSETEDITORRUNTIME_API UDialogEndNodeInfo : public UDialogNodeInfoBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    EDialogNodeAction Action = EDialogNodeAction::None;

    UPROPERTY(EditAnywhere)
    FString ActionData = TEXT("");
};
