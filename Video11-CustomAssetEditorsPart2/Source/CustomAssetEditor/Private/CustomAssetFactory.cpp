#include "CustomAssetFactory.h"
#include "CustomAsset.h"

UCustomAssetFactory::UCustomAssetFactory(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
	SupportedClass = UCustomAsset::StaticClass();
}

UObject* UCustomAssetFactory::FactoryCreateNew(UClass* uclass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn) {
    UCustomAsset* asset = NewObject<UCustomAsset>(inParent, name, flags);
	return asset;
}

bool UCustomAssetFactory::CanCreateNew() const {
    return true;
}
