#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogEndNodeInfo.h"
#include "DialogGraphNodeBase.h"
#include "DialogEndGraphNode.generated.h"

UCLASS()
class UDialogEndGraphNode : public UDialogGraphNodeBase {
    GENERATED_BODY()

public: // UDialogStartGraphNode interface
    virtual FText GetNodeTitle(ENodeTitleType::Type titalType) const override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Blue); }
    virtual bool CanUserDeleteNode() const override { return true; }
    virtual void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const override;

public: // UDialogGraphNodeBase interface
    virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection direction, FName name) override;
    virtual UEdGraphPin* CreateDefaultInputPin() override; 

    virtual void InitNodeInfo(UObject* outer) { _nodeInfo = NewObject<UDialogEndNodeInfo>(outer); }
    virtual void SetNodeInfo(UDialogNodeInfoBase* nodeInfo) override { _nodeInfo = Cast<UDialogEndNodeInfo>(nodeInfo); }
    virtual UDialogNodeInfoBase* GetNodeInfo() const override { return _nodeInfo; }

    virtual EDialogNodeType GetDialogNodeType() const override { return EDialogNodeType::EndNode; }

    virtual void OnPropertiesChanged() { Modify(); }

protected:
    UPROPERTY()
    class UDialogEndNodeInfo* _nodeInfo = nullptr;
};
