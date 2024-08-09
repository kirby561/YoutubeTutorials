#include "DialogAssetPrimaryTabFactory.h"
#include "DialogAssetEditorApp.h"
#include "DialogAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

DialogAssetPrimaryTabFactory::DialogAssetPrimaryTabFactory(TSharedPtr<DialogAssetEditorApp> app) : FWorkflowTabFactory(FName("DialogAssetPrimaryTab"), app) {
    _app = app;

    TabLabel = FText::FromString(TEXT("Primary"));

	ViewMenuDescription = FText::FromString(TEXT("Displays a primary view for whatever you want to do."));
	ViewMenuTooltip = FText::FromString(TEXT("Show the primary view."));
}

TSharedRef<SWidget> DialogAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    TSharedPtr<DialogAssetEditorApp> app = _app.Pin();

    SGraphEditor::FGraphEditorEvents graphEvents;
    graphEvents.OnSelectionChanged.BindRaw(app.Get(), &DialogAssetEditorApp::OnGraphSelectionChanged);

    TSharedPtr<SGraphEditor> graphEditor = 
        SNew(SGraphEditor)
            .IsEditable(true)
            .GraphEvents(graphEvents)
            .GraphToEdit(app->GetWorkingGraph());
    app->SetWorkingGraphUi(graphEditor);

    return SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                .HAlign(HAlign_Fill)
                [
                    graphEditor.ToSharedRef()
                ];
}

FText DialogAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const {
    return FText::FromString(TEXT("A primary view for doing primary things."));
}
