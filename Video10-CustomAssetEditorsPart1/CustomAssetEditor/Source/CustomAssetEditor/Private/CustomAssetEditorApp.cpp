#include "CustomAssetEditorApp.h"
#include "CustomAssetAppMode.h"
#include "CustomAsset.h"

void CustomAssetEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) {
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void CustomAssetEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject) {
	TArray<UObject*> objectsToEdit;
    objectsToEdit.Add(inObject);
    
    _workingAsset = Cast<UCustomAsset>(inObject);

	InitAssetEditor( 
        mode, 
        initToolkitHost, 
        TEXT("CustomAssetEditor"), 
        FTabManager::FLayout::NullLayout, 
        true, // createDefaultStandaloneMenu 
        true,  // createDefaultToolbar
        objectsToEdit);

    // Add our modes (just one for this example)
    AddApplicationMode(TEXT("CustomAssetAppMode"), MakeShareable(new CustomAssetAppMode(SharedThis(this))));

    // Set the mode
    SetCurrentMode(TEXT("CustomAssetAppMode"));
}
