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

    void UpdateOrbitCamera();

    void AddOrbitInput(
        float YawInput,
        float PitchInput);

private:

    void UpdateCursorTrace();

    APlayerCharacter* GetPlayer() const;


protected:

    float OrbitYaw = 0.f;
    float OrbitPitch = 10.f;

private:

    UPROPERTY()
    TObjectPtr<AActor> CurrentTarget = nullptr;

    UPROPERTY(EditAnywhere, Category = "Observation")
    float OrbitSensitivity = 2.f;
};