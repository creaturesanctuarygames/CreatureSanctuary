#pragma once

#include "CoreMinimal.h"
#include "CreatureBodyPart.generated.h"

UENUM(BlueprintType)
enum class ECreatureBodyPart : uint8
{
    Head            UMETA(DisplayName = "Head"),
    Body            UMETA(DisplayName = "Body"),

    LeftFrontLeg    UMETA(DisplayName = "Left Front Leg"),
    RightFrontLeg   UMETA(DisplayName = "Right Front Leg"),

    LeftBackLeg     UMETA(DisplayName = "Left Back Leg"),
    RightBackLeg    UMETA(DisplayName = "Right Back Leg"),

    Tail            UMETA(DisplayName = "Tail")
};