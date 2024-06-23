#include "CustomAssetEditorApp.h"
#include "CustomAssetAppMode.h"
#include "CustomAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "CustomGraphSchema.h"
#include "CustomGraphNode.h"

void CustomAssetEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) {
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void CustomAssetEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject) {
	TArray<UObject*> objectsToEdit;
    objectsToEdit.Add(inObject);
    
    _workingAsset = Cast<UCustomAsset>(inObject);

    // Create an editor graph
    _workingGraph = FBlueprintEditorUtils::CreateNewGraph(
        _workingAsset, 
        NAME_None, 
        UEdGraph::StaticClass(), 
        UCustomGraphSchema::StaticClass()
    );

	InitAssetEditor( 
        mode, 
        initToolkitHost, 
        TEXT("CustomAssetEditor"), 
        FTabManager::FLayout::NullLayout, 
        true, // createDefaultStandaloneMenu 
        true,  // createDefaultToolbar
        objectsToEdit);

    // Add our modes (just one for this example)
    AddApplicationMode(TEXT("CustomAssetAppMode"), MakeShareable(new CustomAssetAppMode(SharedThis(this))));

    // Set the mode
    SetCurrentMode(TEXT("CustomAssetAppMode"));

    // Load the UI from the asset
    UpdateEditorGraphFromWorkingAsset();

    _graphChangeListenerHandle = _workingGraph->AddOnGraphChangedHandler(
        FOnGraphChanged::FDelegate::CreateSP(this, &CustomAssetEditorApp::OnGraphChanged)
    );
}

void CustomAssetEditorApp::OnClose() {
    UpdateWorkingAssetFromGraph();
    _workingGraph->RemoveOnGraphChangedHandler(_graphChangeListenerHandle);
    FAssetEditorToolkit::OnClose();
}

void CustomAssetEditorApp::OnGraphChanged(const FEdGraphEditAction& editAction) {
    UpdateWorkingAssetFromGraph();
}

void CustomAssetEditorApp::UpdateWorkingAssetFromGraph() {
    if (_workingAsset == nullptr || _workingGraph == nullptr) return;

    // Update the state we need into our saveable format
    UCustomRuntimeGraph* runtimeGraph = NewObject<UCustomRuntimeGraph>(_workingAsset);
    _workingAsset->Graph = runtimeGraph;

    TArray<std::pair<FGuid, FGuid>> connections;
    TMap<FGuid, UCustomRuntimePin*> idToPinMap;

    // First create all the nodes/pins and record the connections
    for (UEdGraphNode* uiNode : _workingGraph->Nodes) {
        UCustomRuntimeNode* runtimeNode = NewObject<UCustomRuntimeNode>(runtimeGraph);
        runtimeNode->Position = FVector2D(uiNode->NodePosX, uiNode->NodePosY);

        for (UEdGraphPin* uiPin : uiNode->Pins) {
            UCustomRuntimePin* runtimePin = NewObject<UCustomRuntimePin>(runtimeNode);
            runtimePin->PinName = uiPin->PinName;
            runtimePin->PinId = uiPin->PinId;

            // Only record the the output side of the connection since this is a directed graph
            if (uiPin->HasAnyConnections() && uiPin->Direction == EEdGraphPinDirection::EGPD_Output) {
                // Only 1 connection is allowed so just take the first one
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

        runtimeGraph->Nodes.Add(runtimeNode);
    }

    // Now make all the connections
    for (std::pair<FGuid, FGuid> connection : connections) {
        UCustomRuntimePin* pin1 = idToPinMap[connection.first];
        UCustomRuntimePin* pin2 = idToPinMap[connection.second];
        pin1->Connection = pin2;
    }
}

void CustomAssetEditorApp::UpdateEditorGraphFromWorkingAsset() {
    if (_workingAsset->Graph == nullptr) {
        return;
    }

    // Create all the nodes/pins first
    TArray<std::pair<FGuid, FGuid>> connections;
    TMap<FGuid, UEdGraphPin*> idToPinMap;
    for (UCustomRuntimeNode* runtimeNode : _workingAsset->Graph->Nodes) {
        UCustomGraphNode* newNode = NewObject<UCustomGraphNode>(_workingGraph);
        newNode->CreateNewGuid();
        newNode->NodePosX = runtimeNode->Position.X;
        newNode->NodePosY = runtimeNode->Position.Y;

        if (runtimeNode->InputPin != nullptr) {
            UCustomRuntimePin* pin = runtimeNode->InputPin;
            UEdGraphPin* uiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, pin->PinName);
            uiPin->PinId = pin->PinId;

            if (pin->Connection != nullptr) {
                connections.Add(std::make_pair(pin->PinId, pin->Connection->PinId));
            }
            idToPinMap.Add(pin->PinId, uiPin);
        }

        for (UCustomRuntimePin* pin : runtimeNode->OutputPins) {
            UEdGraphPin* uiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, pin->PinName);
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
