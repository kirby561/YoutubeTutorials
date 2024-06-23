#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class CustomAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook {
public: // FWorkflowCentricApplication interface
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject);

    class UCustomAsset* GetWorkingAsset() { return _workingAsset; }
    class UEdGraph* GetWorkingGraph() { return _workingGraph; }

public: // FAssetEditorToolkit interface
    virtual FName GetToolkitFName() const override { return FName(TEXT("CustomAssetEditorApp")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("CustomAssetEditorApp")); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("CustomAssetEditorApp"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f); }
	virtual FString GetDocumentationLink() const override { return TEXT("https://github.com/kirby561"); }
	virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit) override { }
	virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit) override { }

    virtual void OnClose() override;
    void OnGraphChanged(const FEdGraphEditAction& editAction);

protected:
    void UpdateWorkingAssetFromGraph();
    void UpdateEditorGraphFromWorkingAsset();
    
private:
    UPROPERTY()
    class UCustomAsset* _workingAsset = nullptr;

    UPROPERTY()
    class UEdGraph* _workingGraph = nullptr;

    FDelegateHandle _graphChangeListenerHandle;
};
