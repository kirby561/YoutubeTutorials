#pragma once

#include "CoreMinimal.h"
#include "TestPlayerController.generated.h"

UCLASS()
class ATestPlayerController : public APlayerController {
    GENERATED_BODY()
public:
    ATestPlayerController();
    virtual ~ATestPlayerController();

    virtual void BeginPlay() override;
};
