#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreatureBodyPart.h"
#include "ObservationComponent.generated.h"

class IObservationTarget;
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

    bool TreatHoveredInjury(); //TODO MS: This has nothing to do here. Move it.  

private:
    void UpdateHoveredInjury(); //TODO MS: This has nothing to do here. Move it.  
    void DrawInjuryDebug(); //TODO MS: This has nothing to do here. Move it.  

private:

    UPROPERTY()
    TObjectPtr<AActor> CurrentTarget;

    UPROPERTY()
    TObjectPtr<UCameraComponent> ObservationCamera;

    UPROPERTY()
    ECreatureBodyPart HoveredBodyPart = ECreatureBodyPart::Body;

    UPROPERTY()
    bool bHasHoveredInjury = false;

    bool bIsObserving = false;

    void UpdateCursorTrace();

    APlayerCharacter* GetPlayer() const;
};