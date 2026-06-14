#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObservableTarget.generated.h"

UINTERFACE(BlueprintType)
class UObservableTarget : public UInterface
{
    GENERATED_BODY()
};

class IObservableTarget
{
    GENERATED_BODY()

public:

    virtual FTransform GetObservationTransform() const = 0;
};