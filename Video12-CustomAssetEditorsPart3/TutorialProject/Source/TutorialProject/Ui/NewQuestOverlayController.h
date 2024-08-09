#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewQuestOverlayController.generated.h"

UCLASS()
class UNewQuestOverlayController : public UUserWidget {
    GENERATED_BODY()

public: // Methods
    UNewQuestOverlayController(const FObjectInitializer& objectInitializer);
    virtual ~UNewQuestOverlayController() { }

public: // Create Method
    static UNewQuestOverlayController* CreateInstance(APlayerController* playerController);

public: // Properties
// ---------- Generated Properties Section ---------- //
//             (Don't modify manually)              //
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UVerticalBox* TextContainer = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* QuestNameText = nullptr;
// ---------- End Generated Properties Section ---------- //
};

// ---------- Generated Loader Section ---------- //
//             (Don't modify manually)            //
UCLASS()
class UNewQuestOverlayLoader : public UObject {
    GENERATED_BODY()
public:
    UNewQuestOverlayLoader();
    virtual ~UNewQuestOverlayLoader() { }

public:
    UPROPERTY()
    UClass* WidgetTemplate = nullptr;
    static const inline FString WidgetPath = TEXT("/Game/WBP_NewQuestOverlay");
};
// ---------- End Generated Loader Section ---------- //
