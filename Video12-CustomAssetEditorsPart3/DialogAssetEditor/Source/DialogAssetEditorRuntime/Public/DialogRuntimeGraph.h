#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "DialogNodeInfo.h"
#include "DialogNodeType.h"
#include "DialogRuntimeGraph.generated.h"

UCLASS()
class DIALOGASSETEDITORRUNTIME_API UDialogRuntimePin : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY()
    FName PinName;

    UPROPERTY()
    FGuid PinId;

    UPROPERTY()
    UDialogRuntimePin* Connection = nullptr;

    UPROPERTY()
    class UDialogRuntimeNode* Parent = nullptr;
};

UCLASS()
class DIALOGASSETEDITORRUNTIME_API UDialogRuntimeNode : public UObject {
    GENERATED_BODY()

public:
   UPROPERTY()
   EDialogNodeType NodeType = EDialogNodeType::DialogNode;

   UPROPERTY()
   UDialogRuntimePin* InputPin;

   UPROPERTY()
   TArray<UDialogRuntimePin*> OutputPins;

   UPROPERTY()
   FVector2D Position;

   UPROPERTY()
   UDialogNodeInfoBase* NodeInfo = nullptr;
};

UCLASS()
class DIALOGASSETEDITORRUNTIME_API UDialogRuntimeGraph : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY()
    TArray<UDialogRuntimeNode*> Nodes;
};
