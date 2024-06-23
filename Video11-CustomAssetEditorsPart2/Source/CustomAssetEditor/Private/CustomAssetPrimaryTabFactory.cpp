#include "CustomAssetPrimaryTabFactory.h"
#include "CustomAssetEditorApp.h"
#include "CustomAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

CustomAssetPrimaryTabFactory::CustomAssetPrimaryTabFactory(TSharedPtr<CustomAssetEditorApp> app) : FWorkflowTabFactory(FName("CustomAssetPrimaryTab"), app) {
    _app = app;

    TabLabel = FText::FromString(TEXT("Primary"));

	ViewMenuDescription = FText::FromString(TEXT("Displays a primary view for whatever you want to do."));
	ViewMenuTooltip = FText::FromString(TEXT("Show the primary view."));
}

TSharedRef<SWidget> CustomAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    TSharedPtr<CustomAssetEditorApp> app = _app.Pin();

    return SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                .HAlign(HAlign_Fill)
                [
                    SNew(SGraphEditor)
                        .IsEditable(true)
                        .GraphToEdit(app->GetWorkingGraph())
                ];
}

FText CustomAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const {
    return FText::FromString(TEXT("A primary view for doing primary things."));
}
