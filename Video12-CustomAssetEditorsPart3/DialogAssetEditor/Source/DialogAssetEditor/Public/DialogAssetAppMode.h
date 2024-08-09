#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/** Application mode for main behavior tree editing mode */
class DialogAssetAppMode : public FApplicationMode
{
public:
	DialogAssetAppMode(TSharedPtr<class DialogAssetEditorApp> app);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<class DialogAssetEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
};
