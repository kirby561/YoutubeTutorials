#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestDialogUiController.generated.h"

UCLASS()
class UQuestDialogUiController : public UUserWidget {
    GENERATED_BODY()

public: // Methods
    UQuestDialogUiController(const FObjectInitializer& objectInitializer);
    virtual ~UQuestDialogUiController() { }

public: // Create Method
    static UQuestDialogUiController* CreateInstance(APlayerController* playerController);

public: // Properties
// ---------- Generated Properties Section ---------- //
//             (Don't modify manually)              //
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* SpeakersImage = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* SpeakersNameText = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* DialogText = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UHorizontalBox* ResponsesBox = nullptr;
// ---------- End Generated Properties Section ---------- //
};

// ---------- Generated Loader Section ---------- //
//             (Don't modify manually)            //
UCLASS()
class UQuestDialogUiLoader : public UObject {
    GENERATED_BODY()
public:
    UQuestDialogUiLoader();
    virtual ~UQuestDialogUiLoader() { }

public:
    UPROPERTY()
    UClass* WidgetTemplate = nullptr;
    static const inline FString WidgetPath = TEXT("/Game/WBP_QuestDialogUi");
};
// ---------- End Generated Loader Section ---------- //
