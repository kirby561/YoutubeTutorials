#include "DialogAssetAppMode.h"
#include "DialogAssetEditorApp.h"
#include "DialogAssetPrimaryTabFactory.h"
#include "DialogAssetPropertiesTabFactory.h"

DialogAssetAppMode::DialogAssetAppMode(TSharedPtr<DialogAssetEditorApp> app) : FApplicationMode(TEXT("DialogAssetAppMode")) {
    _app = app;
    _tabs.RegisterFactory(MakeShareable(new DialogAssetPrimaryTabFactory(app)));
    _tabs.RegisterFactory(MakeShareable(new DialogAssetPropertiesTabFactory(app)));

    TabLayout = FTabManager::NewLayout("DialogAssetAppMode_Layout_v1")
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
							->AddTab(FName(TEXT("DialogAssetPrimaryTab")), ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
							->SetSizeCoefficient(0.25)
							->AddTab(FName(TEXT("DialogAssetPropertiesTab")), ETabState::OpenedTab)
					)
			)
	);
}

void DialogAssetAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) {
    TSharedPtr<DialogAssetEditorApp> app = _app.Pin();
	app->PushTabFactories(_tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void DialogAssetAppMode::PreDeactivateMode() {
	FApplicationMode::PreDeactivateMode();
}

void DialogAssetAppMode::PostActivateMode() {
	FApplicationMode::PostActivateMode();
}
