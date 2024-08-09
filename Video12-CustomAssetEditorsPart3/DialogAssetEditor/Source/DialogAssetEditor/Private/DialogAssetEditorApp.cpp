#include "DialogAssetEditorApp.h"
#include "DialogAssetAppMode.h"
#include "DialogAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "DialogGraphSchema.h"
#include "DialogGraphNode.h"
#include "DialogStartGraphNode.h"
#include "DialogEndGraphNode.h"
#include "DialogNodeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(DialogAssetEditorAppSub, Log, All);

void DialogAssetEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) {
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void DialogAssetEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject) {
	TArray<UObject*> objectsToEdit;
    objectsToEdit.Add(inObject);
    
    _workingAsset = Cast<UDialogAsset>(inObject);
    _workingAsset->SetPreSaveListener([this] () { OnWorkingAssetPreSave(); });

    _workingGraph = FBlueprintEditorUtils::CreateNewGraph(
        _workingAsset,
        NAME_None,
        UEdGraph::StaticClass(),
        UDialogGraphSchema::StaticClass()
    );

	InitAssetEditor( 
        mode, 
        initToolkitHost, 
        TEXT("DialogAssetEditor"), 
        FTabManager::FLayout::NullLayout, 
        true, // createDefaultStandaloneMenu 
        true,  // createDefaultToolbar
        objectsToEdit);

    // Add our modes (just one for this example)
    AddApplicationMode(TEXT("DialogAssetAppMode"), MakeShareable(new DialogAssetAppMode(SharedThis(this))));

    // Set the mode
    SetCurrentMode(TEXT("DialogAssetAppMode"));

    UpdateEditorGraphFromWorkingAsset();
}

void DialogAssetEditorApp::OnClose() {
    UpdateWorkingAssetFromGraph();
    _workingAsset->SetPreSaveListener(nullptr);
    FAssetEditorToolkit::OnClose();
}

void DialogAssetEditorApp::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& event) {
    if (_workingGraphUi != nullptr) {
        // Get the node being modified
        UDialogGraphNodeBase* dialogNode = GetSelectedNode(_workingGraphUi->GetSelectedNodes());
        if (dialogNode != nullptr) {
            dialogNode->OnPropertiesChanged();
        }
        _workingGraphUi->NotifyGraphChanged();
    }
}

void DialogAssetEditorApp::OnWorkingAssetPreSave() {
    // Update our asset from the graph just before saving it
    UpdateWorkingAssetFromGraph();
}

void DialogAssetEditorApp::UpdateWorkingAssetFromGraph() {
    if (_workingAsset == nullptr || _workingGraph == nullptr) {
        return;
    }

    UDialogRuntimeGraph* runtimeGraph = NewObject<UDialogRuntimeGraph>(_workingAsset);
    _workingAsset->Graph = runtimeGraph;

    TArray<std::pair<FGuid, FGuid>> connections;
    TMap<FGuid, UDialogRuntimePin*> idToPinMap;

    for (UEdGraphNode* uiNode : _workingGraph->Nodes) {
        UDialogRuntimeNode* runtimeNode = NewObject<UDialogRuntimeNode>(runtimeGraph);
        runtimeNode->Position = FVector2D(uiNode->NodePosX, uiNode->NodePosY);

        for (UEdGraphPin* uiPin : uiNode->Pins) {
            UDialogRuntimePin* runtimePin = NewObject<UDialogRuntimePin>(runtimeNode);
            runtimePin->PinName = uiPin->PinName;
            runtimePin->PinId = uiPin->PinId;
            runtimePin->Parent = runtimeNode;

            if (uiPin->HasAnyConnections() && uiPin->Direction == EEdGraphPinDirection::EGPD_Output) {
                std::pair<FGuid, FGuid> connection = std::make_pair(uiPin->PinId, uiPin->LinkedTo[0]->PinId);
                connections.Add(connection);
            }

            idToPinMap.Add(uiPin->PinId, runtimePin);
            if (uiPin->Direction == EEdGraphPinDirection::EGPD_Input) {
                runtimeNode->InputPin = runtimePin;
            } else {
                runtimeNode->OutputPins.Add(runtimePin);
            }
        }

        UDialogGraphNodeBase* uiDialogNode = Cast<UDialogGraphNodeBase>(uiNode);
        runtimeNode->NodeInfo = DuplicateObject(uiDialogNode->GetNodeInfo(), runtimeNode);
        runtimeNode->NodeType = uiDialogNode->GetDialogNodeType();

        runtimeGraph->Nodes.Add(runtimeNode);
    }

    for (std::pair<FGuid, FGuid> connection : connections) {
        UDialogRuntimePin* pin1 = idToPinMap[connection.first];
        UDialogRuntimePin* pin2 = idToPinMap[connection.second];
        pin1->Connection = pin2;
    };
}

void DialogAssetEditorApp::UpdateEditorGraphFromWorkingAsset() {
    if (_workingAsset->Graph == nullptr) {
        UDialogRuntimeGraph* runtimeGraph = NewObject<UDialogRuntimeGraph>(_workingAsset);
        _workingGraph->GetSchema()->CreateDefaultNodesForGraph(*_workingGraph);
        return;
    }

    TArray<std::pair<FGuid, FGuid>> connections;
    TMap<FGuid, UEdGraphPin*> idToPinMap;

    for (UDialogRuntimeNode* runtimeNode : _workingAsset->Graph->Nodes) {
        UDialogGraphNodeBase* newNode = nullptr;
        if (runtimeNode->NodeType == EDialogNodeType::StartNode) {
            newNode = NewObject<UDialogStartGraphNode>(_workingGraph);
        } else if (runtimeNode->NodeType == EDialogNodeType::DialogNode) {
            newNode = NewObject<UDialogGraphNode>(_workingGraph);
        } else if (runtimeNode->NodeType == EDialogNodeType::EndNode) {
            newNode = NewObject<UDialogEndGraphNode>(_workingGraph);
        } else {
            UE_LOG(DialogAssetEditorAppSub, Error, TEXT("DialogAssetEditorApp::UpdateEditorGraphFromWorkingAsset: Unknown node type"));
            continue;
        }
        newNode->CreateNewGuid();
        newNode->NodePosX = runtimeNode->Position.X;
        newNode->NodePosY = runtimeNode->Position.Y;
        
        if (runtimeNode->NodeInfo != nullptr) {
            newNode->SetNodeInfo(DuplicateObject(runtimeNode->NodeInfo, newNode));
        } else {
            newNode->InitNodeInfo(newNode);
        }

        if (runtimeNode->InputPin != nullptr) {
            UDialogRuntimePin* pin = runtimeNode->InputPin;
            UEdGraphPin* uiPin = newNode->CreateDialogPin(EEdGraphPinDirection::EGPD_Input, pin->PinName);
            uiPin->PinId = pin->PinId;

            if (pin->Connection != nullptr) {
                connections.Add(std::make_pair(pin->PinId, pin->Connection->PinId));
            }
            idToPinMap.Add(pin->PinId, uiPin);
        }

        for (UDialogRuntimePin* pin : runtimeNode->OutputPins) {
            UEdGraphPin* uiPin = newNode->CreateDialogPin(EEdGraphPinDirection::EGPD_Output, pin->PinName);
            uiPin->PinId = pin->PinId;

            if (pin->Connection != nullptr) {
                connections.Add(std::make_pair(pin->PinId, pin->Connection->PinId));
            }
            idToPinMap.Add(pin->PinId, uiPin);
        }

        _workingGraph->AddNode(newNode, true, true);
    }

    for (std::pair<FGuid, FGuid> connection : connections) {
        UEdGraphPin* fromPin = idToPinMap[connection.first];
        UEdGraphPin* toPin = idToPinMap[connection.second];
        fromPin->LinkedTo.Add(toPin);
        toPin->LinkedTo.Add(fromPin);
    }
}

/**
 * Returns the first UDialogGraphNode in the given selection or nullptr if there are none.
 */
UDialogGraphNodeBase* DialogAssetEditorApp::GetSelectedNode(const FGraphPanelSelectionSet& selection) {
    for (UObject* obj : selection) {
        UDialogGraphNodeBase* node = Cast<UDialogGraphNodeBase>(obj);
        if (node != nullptr) {
            return node;
        }
    }

    return nullptr;
}

void DialogAssetEditorApp::SetSelectedNodeDetailView(TSharedPtr<class IDetailsView> detailsView) { 
    _selectedNodeDetailView = detailsView;
    _selectedNodeDetailView->OnFinishedChangingProperties().AddRaw(this, &DialogAssetEditorApp::OnNodeDetailViewPropertiesUpdated);
}

void DialogAssetEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& selection) {
    UDialogGraphNodeBase* selectedNode = GetSelectedNode(selection);
    if (selectedNode != nullptr) {
        _selectedNodeDetailView->SetObject(selectedNode->GetNodeInfo());
    } else {
        _selectedNodeDetailView->SetObject(nullptr);
    }
}
