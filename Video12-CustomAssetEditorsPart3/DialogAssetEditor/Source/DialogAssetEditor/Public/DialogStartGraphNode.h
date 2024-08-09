#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogGraphNodeBase.h"
#include "DialogStartGraphNode.generated.h"

UCLASS()
class UDialogStartGraphNode : public UDialogGraphNodeBase {
    GENERATED_BODY()

public: // UDialogStartGraphNode interface
    virtual FText GetNodeTitle(ENodeTitleType::Type titalType) const override { return FText::FromString("Start"); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Red); }
    virtual bool CanUserDeleteNode() const override { return false; }

public: // UDialogGraphNodeBase interface
    virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection direction, FName name) override;

    virtual EDialogNodeType GetDialogNodeType() const override { return EDialogNodeType::StartNode; }
};
