// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAssetEditor.h"
#include "CustomAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FCustomAssetEditorModule"

class SCustomGraphPin : public SGraphPin {
public:
	SLATE_BEGIN_ARGS(SCustomGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) {
		SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj);
	}
	
protected:
	virtual FSlateColor GetPinColor() const override {
		return FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f));
	}
};

struct FCustomPinFactory : public FGraphPanelPinFactory {
public:
	virtual ~FCustomPinFactory() {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* pin) const override {
		if (FName(TEXT("CustomPin")) == pin->PinType.PinSubCategory) {
			return SNew(SCustomGraphPin, pin);
		}
		return nullptr;
	}
};

void FCustomAssetEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& assetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("CustomAssets")), LOCTEXT("CustomAssets", "Custom Assets"));
	TSharedPtr<CustomAssetAction> customAssetAction = MakeShareable(new CustomAssetAction(assetType));
	assetToolsModule.RegisterAssetTypeActions(customAssetAction.ToSharedRef());

	// Make a styleset and create the icon brush for our custom asset
	_styleSet = MakeShareable(new FSlateStyleSet(TEXT("CustomAssetEditorStyle")));
	TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("CustomAssetEditor");
	FString contentDir = plugin->GetContentDir();
	_styleSet->SetContentRoot(contentDir);

	FSlateImageBrush* thumbnailBrush = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("CustomAssetThumbnail"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* iconBrush = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("CustomAssetIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeAddPinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeAddPinIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeDeletePinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeletePinIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* nodeDeleteNodeIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeleteNodeIcon"), TEXT(".png")), FVector2D(128.0f, 128.0f));
	_styleSet->Set(TEXT("ClassThumbnail.CustomAsset"), thumbnailBrush);
	_styleSet->Set(TEXT("ClassIcon.CustomAsset"), iconBrush);
	_styleSet->Set(TEXT("CustomAssetEditor.NodeAddPinIcon"), nodeAddPinIcon);
	_styleSet->Set(TEXT("CustomAssetEditor.NodeDeletePinIcon"), nodeDeletePinIcon);
	_styleSet->Set(TEXT("CustomAssetEditor.NodeDeleteNodeIcon"), nodeDeleteNodeIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*_styleSet);

	// Register a custom pin factory for creating pins the way we want
	_pinFactory = MakeShareable(new FCustomPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(_pinFactory);
}

void FCustomAssetEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(*_styleSet);
	FEdGraphUtilities::UnregisterVisualPinFactory(_pinFactory);
	_pinFactory = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomAssetEditorModule, CustomAssetEditor)