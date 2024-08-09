#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class DialogAssetAction : public FAssetTypeActions_Base {
public:
    DialogAssetAction(EAssetTypeCategories::Type category);

public: // FAssetTypeActions_Base interface
    virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;

private:
    EAssetTypeCategories::Type _assetCategory;
};
