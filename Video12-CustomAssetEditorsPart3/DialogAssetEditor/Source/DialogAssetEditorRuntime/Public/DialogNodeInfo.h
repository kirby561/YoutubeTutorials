#pragma once

#include "CoreMinimal.h"
#include "DialogNodeInfoBase.h"
#include "DialogNodeInfo.generated.h"

UCLASS(BlueprintType)
class DIALOGASSETEDITORRUNTIME_API UDialogNodeInfo : public UDialogNodeInfoBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FText Title;

    UPROPERTY(EditAnywhere)
    FText DialogText;

    UPROPERTY(EditAnywhere)
    TArray<FText> DialogResponses;
};
