#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class DialogAssetPropertiesTabFactory : public FWorkflowTabFactory {
public:
	DialogAssetPropertiesTabFactory(TSharedPtr<class DialogAssetEditorApp> app);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class DialogAssetEditorApp> _app;
};
