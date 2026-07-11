#include "CreatureInspectionComponent.h"

#include "CreatureInjury.h"
#include "ObservableCreature.h"
#include "PlayerCharacter.h"

#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

UCreatureInspectionComponent::UCreatureInspectionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

APlayerCharacter* UCreatureInspectionComponent::GetPlayer() const
{
    return Cast<APlayerCharacter>(GetOwner());
}

void UCreatureInspectionComponent::SetObservedCreature(
    AObservableCreature* Creature)
{
    ObservedCreature = Creature;
    bHasHoveredInjury = false;
}

void UCreatureInspectionComponent::ClearObservedCreature()
{
    ObservedCreature = nullptr;
    bHasHoveredInjury = false;
}

void UCreatureInspectionComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction);

    if (!ObservedCreature)
    {
        return;
    }

    UpdateHoveredInjury();
    DrawInjuryDebug();
}

void UCreatureInspectionComponent::UpdateHoveredInjury()
{
    bHasHoveredInjury = false;

    APlayerCharacter* Player = GetPlayer();

    if (!Player)
    {
        return;
    }

    APlayerController* PC =
        Cast<APlayerController>(Player->GetController());

    if (!PC)
    {
        return;
    }

    FHitResult Hit;

    if (!PC->GetHitResultUnderCursor(
        ECC_Visibility,
        false,
        Hit))
    {
        return;
    }

    USphereComponent* Sphere =
        Cast<USphereComponent>(Hit.GetComponent());

    if (!Sphere)
    {
        return;
    }

    const TArray<FName>& Tags = Sphere->ComponentTags;

    if (Tags.Num() == 0)
    {
        return;
    }

    const FName Tag = Tags[0];

    if (Tag == "Head")
    {
        HoveredBodyPart = ECreatureBodyPart::Head;
    }
    else if (Tag == "Body")
    {
        HoveredBodyPart = ECreatureBodyPart::Body;
    }
    else if (Tag == "LeftFrontLeg")
    {
        HoveredBodyPart = ECreatureBodyPart::LeftFrontLeg;
    }
    else if (Tag == "RightFrontLeg")
    {
        HoveredBodyPart = ECreatureBodyPart::RightFrontLeg;
    }
    else if (Tag == "LeftBackLeg")
    {
        HoveredBodyPart = ECreatureBodyPart::LeftBackLeg;
    }
    else if (Tag == "RightBackLeg")
    {
        HoveredBodyPart = ECreatureBodyPart::RightBackLeg;
    }
    else if (Tag == "Tail")
    {
        HoveredBodyPart = ECreatureBodyPart::Tail;
    }
    else
    {
        return;
    }

    bHasHoveredInjury = true;
}

void UCreatureInspectionComponent::DrawInjuryDebug()
{
    if (!ObservedCreature)
    {
        return;
    }

    for (const FCreatureInjury& Injury : ObservedCreature->ActiveInjuries)
    {
        USphereComponent* Sphere =
            ObservedCreature->GetInjuryPoint(Injury.BodyPart);

        if (!Sphere)
        {
            continue;
        }

        const bool bHovered =
            bHasHoveredInjury &&
            Injury.BodyPart == HoveredBodyPart;

        DrawDebugSphere(
            GetWorld(),
            Sphere->GetComponentLocation(),
            Sphere->GetScaledSphereRadius(),
            16,
            bHovered ? FColor::Yellow : FColor::Green,
            false,
            -1.f,
            0,
            2.f);

        if (bHovered && Injury.Injury)
        {
            DrawDebugString(
                GetWorld(),
                Sphere->GetComponentLocation() + FVector(0, 0, 20),
                Injury.Injury->DisplayName.ToString(),
                nullptr,
                FColor::White,
                0.f,
                true);
        }
    }
}

bool UCreatureInspectionComponent::TreatHoveredInjury()
{
    if (!ObservedCreature)
    {
        return false;
    }

    if (!bHasHoveredInjury)
    {
        return false;
    }

    if (ObservedCreature->RemoveInjury(HoveredBodyPart))
    {
        bHasHoveredInjury = false;

        UE_LOG(LogTemp, Warning, TEXT("Injury treated."));

        return true;
    }

    return false;
}