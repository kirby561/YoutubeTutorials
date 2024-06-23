#include "CustomAssetAppMode.h"
#include "CustomAssetEditorApp.h"
#include "CustomAssetPrimaryTabFactory.h"
#include "CustomAssetPropertiesTabFactory.h"

CustomAssetAppMode::CustomAssetAppMode(TSharedPtr<CustomAssetEditorApp> app) : FApplicationMode(TEXT("CustomAssetAppMode")) {
    _app = app;
    _tabs.RegisterFactory(MakeShareable(new CustomAssetPrimaryTabFactory(app)));
    _tabs.RegisterFactory(MakeShareable(new CustomAssetPropertiesTabFactory(app)));

    TabLayout = FTabManager::NewLayout("CustomAssetAppMode_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewSplitter()
						->SetOrientation(Orient_Horizontal)
						->Split
						(
							FTabManager::NewStack()
								->SetSizeCoefficient(0.75)
								->AddTab(FName(TEXT("CustomAssetPrimaryTab")), ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
								->SetSizeCoefficient(0.25)
								->AddTab(FName(TEXT("CustomAssetPropertiesTab")), ETabState::OpenedTab)
						)
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
