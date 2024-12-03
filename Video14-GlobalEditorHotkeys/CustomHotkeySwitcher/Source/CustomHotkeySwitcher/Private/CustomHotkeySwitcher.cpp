// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomHotkeySwitcher.h"
#include "Interfaces/IMainFrameModule.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"
#include "SplineComponentVisualizer.h"
#include "Components/SplineComponent.h"
#include "EngineUtils.h"
#include <limits>
#include "Framework/Commands/InputBindingManager.h"

DEFINE_LOG_CATEGORY_STATIC(CustomHotkeySwitcherModuleSub, Log, All);

#define LOCTEXT_NAMESPACE "FCustomHotkeySwitcherModule"

void FCustomHotkeySwitcherModule::StartupModule()
{
	FInputBindingManager& inputBindingManager = FInputBindingManager::Get();
	IMainFrameModule& mainFrameModule = IMainFrameModule::Get();
	TSharedRef<FUICommandList>& commandList = mainFrameModule.GetMainFrameCommandBindings();
	FName mainFrameContextName = TEXT("MainFrame");
	TSharedPtr<FBindingContext> mainFrameContext = inputBindingManager.GetContextByName(mainFrameContextName);
	FInputChord defaultInputChord(EModifierKey::Shift, EKeys::F);

	const TSharedPtr<FUICommandInfo> existingCommand = inputBindingManager.GetCommandInfoFromInputChord(
		mainFrameContextName,
		defaultInputChord,
		false // check default
	);
	if (existingCommand.IsValid()) {
		existingCommand->RemoveActiveChord(EMultipleKeyBindingIndex::Primary);
		existingCommand->RemoveActiveChord(EMultipleKeyBindingIndex::Secondary);
	}

	UI_COMMAND_EXT(mainFrameContext.Get(), _customCommandInfo, "CustomCommand1", "Custom Command 1", "A custom command", EUserInterfaceActionType::Button, defaultInputChord);

	commandList->MapAction(
		_customCommandInfo,
		FExecuteAction::CreateRaw(this, &FCustomHotkeySwitcherModule::OnCustomCommand)
	);
}

void FCustomHotkeySwitcherModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FCustomHotkeySwitcherModule::OnCustomCommand() {
	TSharedPtr<FComponentVisualizer> visualizer = GUnrealEd->FindComponentVisualizer(USplineComponent::StaticClass());
	TSharedPtr<FSplineComponentVisualizer> splineVisualizer = StaticCastSharedPtr<FSplineComponentVisualizer>(visualizer);
	if (USplineComponent* splineComponent = splineVisualizer->GetEditedSplineComponent()) {
		// Get all the other spline components in the world that are not this one
		// If your level is going to have a ton of them, maybe sort by location in an octree or something to make it faster
		TArray<USplineComponent*> otherSplines;
		UWorld* editorWorld = splineComponent->GetWorld();
		for (TActorIterator<AActor> actorItr(editorWorld); actorItr; ++actorItr) {
			TArray<USplineComponent*> splineComponents;
			actorItr->GetComponents<USplineComponent>(splineComponents);
			for (USplineComponent* component : splineComponents) {
				if (component != splineComponent) {
					otherSplines.Add(component);
				}
			}
		}

		double targetDistanceSquared = _snapDistance * _snapDistance;
		const TSet<int32>& keys = splineVisualizer->GetSelectedKeys();
		for (int32 key : keys) {
			FVector currentLocation = splineComponent->GetLocationAtSplinePoint(key, ESplineCoordinateSpace::World);
			USplineComponent* closestSpline = nullptr;
			int32 closestSplineIndex = -1;
			double closestDistance = std::numeric_limits<double>::max();
			for (USplineComponent* otherSpline : otherSplines) {
				for (int32 i = 0; i < otherSpline->GetNumberOfSplinePoints(); i++) {
					FVector pointLocation = otherSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
					double distanceSquared = (pointLocation - currentLocation).SizeSquared();
					if (distanceSquared < targetDistanceSquared && distanceSquared < closestDistance) {
						closestSpline = otherSpline;
						closestSplineIndex = i;
						closestDistance = distanceSquared;
					}
				}
			}

			if (closestSpline != nullptr) {
				// Snap it
				FVector snapLocation = closestSpline->GetLocationAtSplinePoint(closestSplineIndex, ESplineCoordinateSpace::World);
				splineComponent->SetWorldLocationAtSplinePoint(key, snapLocation);
			}
		}
	}

	UE_LOG(CustomHotkeySwitcherModuleSub, Display, TEXT("OnCustomCommand"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomHotkeySwitcherModule, CustomHotkeySwitcher)