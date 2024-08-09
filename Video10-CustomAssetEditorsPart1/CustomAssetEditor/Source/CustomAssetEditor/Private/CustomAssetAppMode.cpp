#include "CustomAssetAppMode.h"
#include "CustomAssetEditorApp.h"
#include "CustomAssetPrimaryTabFactory.h"

CustomAssetAppMode::CustomAssetAppMode(TSharedPtr<CustomAssetEditorApp> app) : FApplicationMode(TEXT("CustomAssetAppMode")) {
    _app = app;
    _tabs.RegisterFactory(MakeShareable(new CustomAssetPrimaryTabFactory(app)));

    TabLayout = FTabManager::NewLayout("CustomAssetAppMode_Layout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
            FTabManager::NewStack()
                ->AddTab(FName(TEXT("CustomAssetPrimaryTab")), ETabState::OpenedTab)
		)
	);
}

void CustomAssetAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) {
    TSharedPtr<CustomAssetEditorApp> app = _app.Pin();
	app->PushTabFactories(_tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void CustomAssetAppMode::PreDeactivateMode() {
	FApplicationMode::PreDeactivateMode();
}

void CustomAssetAppMode::PostActivateMode() {
	FApplicationMode::PostActivateMode();
}
