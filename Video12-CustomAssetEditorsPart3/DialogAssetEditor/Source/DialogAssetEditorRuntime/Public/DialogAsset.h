#pragma once

#include "CoreMinimal.h"
#include "DialogRuntimeGraph.h"
#include <functional>
#include "DialogAsset.generated.h"

UCLASS(BlueprintType)
class DIALOGASSETEDITORRUNTIME_API UDialogAsset : public UObject {
    GENERATED_BODY()

public: // Properties
    UPROPERTY(EditAnywhere)
    FString DialogName = TEXT("Enter dialog name here");

    UPROPERTY()
    UDialogRuntimeGraph* Graph = nullptr;

public: // Our interface
    void SetPreSaveListener(std::function<void()> onPreSaveListener) { _onPreSaveListener = onPreSaveListener; }

public: // UObject interface
    virtual void PreSave(FObjectPreSaveContext saveContext) override;

private: // Members
    std::function<void()> _onPreSaveListener = nullptr;
};
