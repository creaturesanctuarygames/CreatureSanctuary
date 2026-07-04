#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CreatureBodyPart.h"
#include "InjuryPointComponent.generated.h"

UCLASS(ClassGroup = (Creature), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UInjuryPointComponent : public USceneComponent
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Injury")
    ECreatureBodyPart BodyPart = ECreatureBodyPart::Body;
};