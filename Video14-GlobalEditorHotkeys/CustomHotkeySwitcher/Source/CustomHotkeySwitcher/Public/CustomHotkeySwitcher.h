// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCustomHotkeySwitcherModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected: // Methods
	void OnCustomCommand();

protected: // Members
	double _snapDistance = 500;
	TSharedPtr<class FUICommandInfo> _customCommandInfo = nullptr;
};
