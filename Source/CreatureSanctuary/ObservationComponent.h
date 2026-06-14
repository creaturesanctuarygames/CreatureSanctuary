#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservationComponent.generated.h"

class IObservableTarget;
class UCameraComponent;
class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UObservationComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UObservationComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void BeginObservation(AActor* Target);
    void EndObservation();

    bool IsObserving() const { return bIsObserving; }

    void UpdateCamera();

private:

    UPROPERTY()
    TObjectPtr<AActor> CurrentTarget;

    UPROPERTY()
    TObjectPtr<UCameraComponent> ObservationCamera;

    bool bIsObserving = false;

    void UpdateCursorTrace();

    APlayerCharacter* GetPlayer() const;
};