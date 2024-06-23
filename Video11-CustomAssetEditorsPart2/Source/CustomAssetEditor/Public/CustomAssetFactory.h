#pragma once

#include "CoreMinimal.h"
#include "CustomAssetFactory.generated.h"

UCLASS()
class UCustomAssetFactory : public UFactory {
    GENERATED_BODY()

public:
    UCustomAssetFactory(const FObjectInitializer& objectInitializer);

public: // UFactory interface
    virtual UObject* FactoryCreateNew(UClass* uclass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn) override;
    virtual bool CanCreateNew() const override;
};
