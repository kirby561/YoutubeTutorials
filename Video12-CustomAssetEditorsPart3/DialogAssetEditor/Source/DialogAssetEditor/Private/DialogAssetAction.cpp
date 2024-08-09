#include "DialogAssetAction.h"
#include "DialogAsset.h"
#include "DialogAssetEditorApp.h"

DialogAssetAction::DialogAssetAction(EAssetTypeCategories::Type category) {
    _assetCategory = category;
}

FText DialogAssetAction::GetName() const {
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_MyDialogAsset", "My Dialog Asset");
}

FColor DialogAssetAction::GetTypeColor() const {
    return FColor::Cyan;
}

UClass* DialogAssetAction::GetSupportedClass() const {
    return UDialogAsset::StaticClass();
}

void DialogAssetAction::OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor) {
    EToolkitMode::Type mode = editWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : inObjects) {
		UDialogAsset* dialogAsset = Cast<UDialogAsset>(object);
		if (dialogAsset != nullptr) {
			TSharedRef<DialogAssetEditorApp> editor(new DialogAssetEditorApp());
			editor->InitEditor(mode, editWithinLevelEditor, dialogAsset);
		}
	}
}

uint32 DialogAssetAction::GetCategories() {
    return _assetCategory;
}
