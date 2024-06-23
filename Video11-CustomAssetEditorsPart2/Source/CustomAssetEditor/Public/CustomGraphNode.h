#pragma once

#include "EdGraph/EdGraphNode.h"
#include "CustomGraphNode.generated.h"

UCLASS()
class UCustomGraphNode : public UEdGraphNode {
    GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("MyNodeTitle"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Green); }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context) const override;

    UEdGraphPin* CreateCustomPin(EEdGraphPinDirection direction, FName name);
};
