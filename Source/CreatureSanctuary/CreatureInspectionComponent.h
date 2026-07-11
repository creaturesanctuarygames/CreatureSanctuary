#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreatureBodyPart.h"
#include "CreatureInspectionComponent.generated.h"

class AObservableCreature;
class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UCreatureInspectionComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UCreatureInspectionComponent();

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    void SetObservedCreature(AObservableCreature* Creature);

    void ClearObservedCreature();

    bool TreatHoveredInjury();

private:

    void UpdateHoveredInjury();

    void DrawInjuryDebug();

    APlayerCharacter* GetPlayer() const;

private:

    UPROPERTY()
    TObjectPtr<AObservableCreature> ObservedCreature = nullptr;

    UPROPERTY()
    ECreatureBodyPart HoveredBodyPart = ECreatureBodyPart::Body;

    UPROPERTY()
    bool bHasHoveredInjury = false;
};