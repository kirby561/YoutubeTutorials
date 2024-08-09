#pragma once

#include "CoreMinimal.h"
#include "CustomAsset.generated.h"

UCLASS(BlueprintType)
class CUSTOMASSETEDITORRUNTIME_API UCustomAsset : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString SomeData = TEXT("DefaultData");
    
    UPROPERTY(EditAnywhere)
    int32 SomeNumber = 0;
    
    UPROPERTY(EditAnywhere)
    bool SomeBool = false;
};
