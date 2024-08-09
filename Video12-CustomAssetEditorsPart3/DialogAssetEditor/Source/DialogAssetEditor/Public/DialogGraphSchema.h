#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "DialogGraphSchema.generated.h"

UCLASS()
class UDialogGraphSchema : public UEdGraphSchema {
    GENERATED_BODY()

public:
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& graph) const override;
};

USTRUCT()
struct FNewNodeAction : public FEdGraphSchemaAction {
    GENERATED_BODY()

public:
    FNewNodeAction() {}
    FNewNodeAction(UClass* classTemplate, FText inNodeCategory, FText inMenuDesc, FText inToolTip, const int32 inGrouping)
        : FEdGraphSchemaAction(inNodeCategory, inMenuDesc, inToolTip, inGrouping), _classTemplate(classTemplate) {}

    virtual UEdGraphNode* PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode = true);

protected:
    UClass* _classTemplate = nullptr;
};
