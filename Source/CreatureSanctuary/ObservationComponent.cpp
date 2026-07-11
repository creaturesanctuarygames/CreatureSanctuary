#include "ObservationComponent.h"

#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ObservationTarget.h"
#include "ObservableCreature.h"
#include "CreatureInjury.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

UObservationComponent::UObservationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

APlayerCharacter* UObservationComponent::GetPlayer() const
{
    return Cast<APlayerCharacter>(GetOwner());
}

void UObservationComponent::BeginObservation(AActor* Target)
{
    if (!Target) return;

    APlayerCharacter* Player = GetPlayer();
    if (!Player) return;

    Player->GetCharacterMovement()->DisableMovement();
    Player->GetCharacterMovement()->StopMovementImmediately();

    APlayerController* PC =
        Cast<APlayerController>(Player->GetController());

    if (!PC) return;

    PC->SetIgnoreMoveInput(true);
    PC->SetIgnoreLookInput(true);

    CurrentTarget = Target;
    bIsObserving = true;

    Player->GetCharacterMovement()->DisableMovement();
    Player->GetCharacterMovement()->StopMovementImmediately();

    PC->bShowMouseCursor = true;

    FInputModeGameAndUI Mode;
    Mode.SetHideCursorDuringCapture(false);
    Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    PC->SetInputMode(Mode);
}

void UObservationComponent::EndObservation()
{
    APlayerCharacter* Player = GetPlayer();
    if (!Player) return;

    APlayerController* PC =
        Cast<APlayerController>(Player->GetController());

    if (!PC) return;

    PC->SetIgnoreMoveInput(false);
    PC->SetIgnoreLookInput(false);

    bIsObserving = false;
    CurrentTarget = nullptr;

    Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

    PC->SetInputMode(FInputModeGameOnly());
    PC->bShowMouseCursor = false;
}

void UObservationComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsObserving)
    {
        return;
    }

    UpdateCursorTrace();
    UpdateHoveredInjury();
    DrawInjuryDebug();
}

void UObservationComponent::UpdateCursorTrace()
{
    APlayerCharacter* Player = GetPlayer();
    if (!Player) return;

    APlayerController* PC =
        Cast<APlayerController>(Player->GetController());

    if (!PC) return;

    FHitResult Hit;

    if (PC->GetHitResultUnderCursor(
        ECC_Visibility,
        false,
        Hit))
    {
        AActor* HitActor = Hit.GetActor();

        if (HitActor)
        {
            DrawDebugSphere(
                GetWorld(),
                Hit.ImpactPoint,
                10.f,
                8,
                FColor::Green);
        }
    }
}

void UObservationComponent::UpdateCamera()
{
    if (!CurrentTarget || !ObservationCamera)
    {
        return;
    }

    IObservationTarget* Target =
        Cast<IObservationTarget>(CurrentTarget);

    if (!Target)
    {
        return;
    }

    const FVector TargetLocation =
        Target->GetObservationTransform().GetLocation();

    const FVector CameraLocation =
        TargetLocation
        - FVector(200.f, 0.f, -80.f); // simple stable offset

    const FRotator CameraRotation =
        (TargetLocation - CameraLocation).Rotation();

    ObservationCamera->SetWorldLocation(CameraLocation);
    ObservationCamera->SetWorldRotation(CameraRotation);
}

void UObservationComponent::DrawInjuryDebug()
{
    AObservableCreature* Creature =
        Cast<AObservableCreature>(CurrentTarget);

    if (!Creature)
    {
        return;
    }

    for (const FCreatureInjury& Injury : Creature->ActiveInjuries)
    {
        USphereComponent* Sphere =
            Creature->GetInjuryPoint(Injury.BodyPart);

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

void UObservationComponent::UpdateHoveredInjury()
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

    //TODO MS: OMG we need an object later where we just get that! 
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

bool UObservationComponent::TreatHoveredInjury()
{
    if (!bHasHoveredInjury)
    {
        return false;
    }

    AObservableCreature* Creature =
        Cast<AObservableCreature>(CurrentTarget);

    if (!Creature)
    {
        return false;
    }

    if (Creature->RemoveInjury(HoveredBodyPart))
    {
        bHasHoveredInjury = false;

        UE_LOG(LogTemp, Warning, TEXT("Injury treated."));
        return true;
    }

    return false;
}
