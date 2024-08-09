#pragma once

#include "CoreMinimal.h"
#include "DialogAssetFactory.generated.h"

UCLASS()
class UDialogAssetFactory : public UFactory {
    GENERATED_BODY()

public:
    UDialogAssetFactory(const FObjectInitializer& objectInitializer);

public: // UFactory interface
    virtual UObject* FactoryCreateNew(UClass* uclass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn) override;
    virtual bool CanCreateNew() const override;
};
