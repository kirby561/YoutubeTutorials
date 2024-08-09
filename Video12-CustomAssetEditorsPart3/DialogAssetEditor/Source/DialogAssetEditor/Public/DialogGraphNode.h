#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogGraphNodeBase.h"
#include "DialogNodeInfo.h"
#include "DialogGraphNode.generated.h"

UCLASS()
class UDialogGraphNode : public UDialogGraphNodeBase {
    GENERATED_BODY()

public: // UEdGraphNode interface
    virtual FText GetNodeTitle(ENodeTitleType::Type titalType) const override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Green); }
    virtual bool CanUserDeleteNode() const override { return true; }
    virtual void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const override;

public: // UDialogGraphNodeBase interface
    virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection direction, FName name) override;
    virtual UEdGraphPin* CreateDefaultInputPin() override;
    virtual void CreateDefaultOutputPins() override;

    virtual EDialogNodeType GetDialogNodeType() const override { return EDialogNodeType::DialogNode; }

    virtual void OnPropertiesChanged() { SyncPinsWithResponses(); }

public: // Our interface
    void SyncPinsWithResponses();

    virtual void InitNodeInfo(UObject* outer) { _nodeInfo = NewObject<UDialogNodeInfo>(outer); }
    virtual void SetNodeInfo(UDialogNodeInfoBase* nodeInfo) override { _nodeInfo = Cast<UDialogNodeInfo>(nodeInfo); }
    virtual UDialogNodeInfoBase* GetNodeInfo() const override { return _nodeInfo; }
    UDialogNodeInfo* GetDialogNodeInfo() { return _nodeInfo; }

protected:
    UPROPERTY()
    class UDialogNodeInfo* _nodeInfo = nullptr;
};
