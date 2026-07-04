#pragma once

#include "CoreMinimal.h"
#include "InjuryDataAsset.h"
#include "CreatureBodyPart.h"
#include "CreatureInjury.generated.h"

USTRUCT(BlueprintType)
struct FCreatureInjury
{
    GENERATED_BODY()

public:

    // Which injury this is (Scratch, Burn, ...)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Injury")
    UInjuryDataAsset* Injury = nullptr;

    // Name of the Scene Component on the creature
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Injury")
    ECreatureBodyPart BodyPart = ECreatureBodyPart::Body;
};