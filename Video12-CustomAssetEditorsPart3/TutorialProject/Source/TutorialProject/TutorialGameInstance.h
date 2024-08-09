#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TutorialGameInstance.generated.h"

UCLASS()
class UTutorialGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UTutorialGameInstance();
	virtual ~UTutorialGameInstance();
};
