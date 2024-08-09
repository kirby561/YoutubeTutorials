#include "DialogAssetFactory.h"
#include "DialogAsset.h"

UDialogAssetFactory::UDialogAssetFactory(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
	SupportedClass = UDialogAsset::StaticClass();
}

UObject* UDialogAssetFactory::FactoryCreateNew(UClass* uclass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn) {
    UDialogAsset* asset = NewObject<UDialogAsset>(inParent, name, flags);
	return asset;
}

bool UDialogAssetFactory::CanCreateNew() const {
    return true;
}
