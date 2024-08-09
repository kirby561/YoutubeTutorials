#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/** Application mode for main behavior tree editing mode */
class CustomAssetAppMode : public FApplicationMode
{
public:
	CustomAssetAppMode(TSharedPtr<class CustomAssetEditorApp> app);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<class CustomAssetEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
};
