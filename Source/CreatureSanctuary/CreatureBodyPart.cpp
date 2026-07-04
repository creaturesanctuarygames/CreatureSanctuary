#include "CreatureBodyPart.h"

FName CreatureBodyPart::ToTag(ECreatureBodyPart BodyPart)
{
    switch (BodyPart)
    {
    case ECreatureBodyPart::Head:
        return "Head";

    case ECreatureBodyPart::Body:
        return "Body";

    case ECreatureBodyPart::LeftFrontLeg:
        return "LeftFrontLeg";

    case ECreatureBodyPart::RightFrontLeg:
        return "RightFrontLeg";

    case ECreatureBodyPart::LeftBackLeg:
        return "LeftBackLeg";

    case ECreatureBodyPart::RightBackLeg:
        return "RightBackLeg";

    case ECreatureBodyPart::Tail:
        return "Tail";
    }

    return NAME_None;
}