#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservationComponent.generated.h"

class UCameraComponent;
class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UObservationComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UObservationComponent();

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    void BeginObservation(AActor* Target);

    void EndObservation();

    void UpdateCamera();

private:

    void UpdateCursorTrace();

    APlayerCharacter* GetPlayer() const;

private:

    UPROPERTY()
    TObjectPtr<AActor> CurrentTarget = nullptr;

    UPROPERTY()
    TObjectPtr<UCameraComponent> ObservationCamera = nullptr;
};