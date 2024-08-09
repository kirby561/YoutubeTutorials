#include "CustomAssetAction.h"
#include "CustomAsset.h"
#include "CustomAssetEditorApp.h"

CustomAssetAction::CustomAssetAction(EAssetTypeCategories::Type category) {
    _assetCategory = category;
}

FText CustomAssetAction::GetName() const {
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_MyCustomAsset", "My Custom Asset");
}

FColor CustomAssetAction::GetTypeColor() const {
    return FColor::Cyan;
}

UClass* CustomAssetAction::GetSupportedClass() const {
    return UCustomAsset::StaticClass();
}

void CustomAssetAction::OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor) {
    EToolkitMode::Type mode = editWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : inObjects) {
		UCustomAsset* customAsset = Cast<UCustomAsset>(object);
		if (customAsset != nullptr) {
			TSharedRef<CustomAssetEditorApp> editor(new CustomAssetEditorApp());
			editor->InitEditor(mode, editWithinLevelEditor, customAsset);
		}
	}
}

uint32 CustomAssetAction::GetCategories() {
    return _assetCategory;
}
