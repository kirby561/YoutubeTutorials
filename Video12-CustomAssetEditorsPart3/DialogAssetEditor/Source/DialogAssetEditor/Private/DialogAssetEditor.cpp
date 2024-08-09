// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogAssetEditor.h"
#include "DialogAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FDialogAssetEditorModule"

class SDialogGraphPin : public SGraphPin {
public:
	SLATE_BEGIN_ARGS(SDialogGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) {
		SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj);
	}

protected:
	virtual FSlateColor GetPinColor() const override {
		return FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f));
	}
};

class SDialogStartGraphPin : public SGraphPin {
public:
	SLATE_BEGIN_ARGS(SDialogGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) {
		SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj);
	}

protected:
	virtual FSlateColor GetPinColor() const override {
		return FSlateColor(FLinearColor(1.0f, 0.2f, 0.2f));
	}
};

class SDialogEndGraphPin : public SGraphPin {
public:
	SLATE_BEGIN_ARGS(SDialogGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) {
		SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj);
	}

protected:
	virtual FSlateColor GetPinColor() const override {
		return FSlateColor(FLinearColor(0.2f, 0.2f, 1.0f));
	}
};

struct FDialogPinFactory : public FGraphPanelPinFactory {
public:
	virtual ~FDialogPinFactory() {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* pin) const override {
		if (FName(TEXT("DialogPin")) == pin->PinType.PinSubCategory) {
			return SNew(SDialogGraphPin, pin);
		} else if (FName(TEXT("StartPin")) == pin->PinType.PinSubCategory) {
			return SNew(SDialogStartGraphPin, pin);
		} else if (FName(TEXT("EndPin")) == pin->PinType.PinSubCategory) {
			return SNew(SDialogEndGraphPin, pin);
		} 

		return nullptr;
	}
};

void FDialogAssetEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& assetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("DialogAssets")), LOCTEXT("DialogAssets", "Dialog Assets"));
	TSharedPtr<DialogAssetAction> dialogAssetAction = MakeShareable(new DialogAssetAction(assetType));
	assetToolsModule.RegisterAssetTypeActions(dialogAssetAction.ToSharedRef());

	_styleSet = MakeShareable(new FSlateStyleSet(TEXT("DialogAssetEditorStyle")));
	TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("DialogAssetEditor");
	FString contentDir = plugin->GetContentDir();
	_styleSet->SetContentRoot(contentDir);

	FSlateImageBrush* thumbnailBrush = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("DialogAssetThumbnail"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* iconBrush = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("DialogAssetIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* nodeAddIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeAddPinIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* nodeDeletePinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeletePinIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* nodeDeleteNodeIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeleteNodeIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	_styleSet->Set(TEXT("ClassThumbnail.DialogAsset"), thumbnailBrush);
	_styleSet->Set(TEXT("ClassIcon.DialogAsset"), iconBrush);
	_styleSet->Set(TEXT("DialogAssetEditor.NodeAddPinIcon"), nodeAddIcon);
	_styleSet->Set(TEXT("DialogAssetEditor.NodeDeletePinIcon"), nodeDeletePinIcon);
	_styleSet->Set(TEXT("DialogAssetEditor.NodeDeleteNodeIcon"), nodeDeleteNodeIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*_styleSet);

	_pinFactory = MakeShareable(new FDialogPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(_pinFactory);
}

void FDialogAssetEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(*_styleSet);
	FEdGraphUtilities::UnregisterVisualPinFactory(_pinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogAssetEditorModule, DialogAssetEditor)