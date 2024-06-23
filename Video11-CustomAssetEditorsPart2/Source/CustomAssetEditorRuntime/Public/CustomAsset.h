#pragma once

#include "CoreMinimal.h"
#include "CustomRuntimeGraph.h"
#include "CustomAsset.generated.h"

UCLASS(BlueprintType)
class CUSTOMASSETEDITORRUNTIME_API UCustomAsset : public UObject {
    GENERATED_BODY()

public: // Properties
    UPROPERTY(EditAnywhere)
    FString SomeData = TEXT("DefaultData");

    UPROPERTY(EditAnywhere)
    int32 SomeNumber = 0;

    UPROPERTY(EditAnywhere)
    bool SomeBool = false;

    UPROPERTY()
    UCustomRuntimeGraph* Graph = nullptr;
};
