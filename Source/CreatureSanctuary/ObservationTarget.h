#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObservationTarget.generated.h"

UINTERFACE(BlueprintType)
class UObservationTarget : public UInterface
{
    GENERATED_BODY()
};

class IObservationTarget
{
    GENERATED_BODY()

public:

    virtual void OnInteract(AActor* InteractingActor) = 0;

    virtual FTransform GetObservationTransform() const = 0;
};