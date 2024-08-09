#pragma once

#include "DialogNodeType.generated.h"

UENUM()
enum class EDialogNodeType {
    Unknown,
	StartNode,
	DialogNode,
	EndNode
};
