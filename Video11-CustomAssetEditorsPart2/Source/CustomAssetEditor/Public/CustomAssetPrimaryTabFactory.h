#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class CustomAssetPrimaryTabFactory : public FWorkflowTabFactory {
public:
	CustomAssetPrimaryTabFactory(TSharedPtr<class CustomAssetEditorApp> app);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class CustomAssetEditorApp> _app;
};
