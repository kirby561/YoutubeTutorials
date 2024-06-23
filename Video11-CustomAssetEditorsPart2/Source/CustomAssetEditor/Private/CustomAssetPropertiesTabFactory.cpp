#include "CustomAssetPropertiesTabFactory.h"
#include "CustomAssetEditorApp.h"
#include "CustomAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

CustomAssetPropertiesTabFactory::CustomAssetPropertiesTabFactory(TSharedPtr<CustomAssetEditorApp> app) : FWorkflowTabFactory(FName("CustomAssetPropertiesTab"), app) {
    _app = app;

    TabLabel = FText::FromString(TEXT("Properties"));

	ViewMenuDescription = FText::FromString(TEXT("Displays the properties of the object being edited."));
	ViewMenuTooltip = FText::FromString(TEXT("Show the properties view."));
}

TSharedRef<SWidget> CustomAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    TSharedPtr<CustomAssetEditorApp> app = _app.Pin();
    FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

    FDetailsViewArgs detailsViewArgs;
    {
        detailsViewArgs.bAllowSearch = false;
        detailsViewArgs.bHideSelectionTip = true;
        detailsViewArgs.bLockable = false;
        detailsViewArgs.bSearchInitialKeyFocus = true;
        detailsViewArgs.bUpdatesFromSelection = false;
        detailsViewArgs.NotifyHook = nullptr;
        detailsViewArgs.bShowOptions = true;
        detailsViewArgs.bShowModifiedPropertiesOption = false;
        detailsViewArgs.bShowScrollBar = false;
    }

    TSharedPtr<IDetailsView> detailsView = propertyEditorModule.CreateDetailView(detailsViewArgs);
    detailsView->SetObject(app->GetWorkingAsset());

    return SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .FillHeight(1.0f)
                .HAlign(HAlign_Fill)
                [
                    detailsView.ToSharedRef()
                ];
}

FText CustomAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const {
    return FText::FromString(TEXT("Displays the properties of the custom asset."));
}
