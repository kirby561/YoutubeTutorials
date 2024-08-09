#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <functional>
#include "QuestResponseButtonController.generated.h"

UCLASS()
class UQuestResponseButtonController : public UUserWidget {
    GENERATED_BODY()

public: // Methods
    UQuestResponseButtonController(const FObjectInitializer& objectInitializer);
    virtual ~UQuestResponseButtonController() { }

    void SetClickHandler(int index, std::function<void(int)> onClickHandler);

    UFUNCTION()
    void OnButtonClicked();

public: // Create Method
    static UQuestResponseButtonController* CreateInstance(APlayerController* playerController);

public: // Properties
// ---------- Generated Properties Section ---------- //
//             (Don't modify manually)              //
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ResponseButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* ResponseButtonText = nullptr;
// ---------- End Generated Properties Section ---------- //

private:
    int _index;
    std::function<void(int)> _onClickHandler;
};

// ---------- Generated Loader Section ---------- //
//             (Don't modify manually)            //
UCLASS()
class UQuestResponseButtonLoader : public UObject {
    GENERATED_BODY()
public:
    UQuestResponseButtonLoader();
    virtual ~UQuestResponseButtonLoader() { }

public:
    UPROPERTY()
    UClass* WidgetTemplate = nullptr;
    static const inline FString WidgetPath = TEXT("/Game/WBP_QuestResponseButton");
};
// ---------- End Generated Loader Section ---------- //
