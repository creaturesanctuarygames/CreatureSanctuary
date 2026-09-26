#include "ObservationComponent.h"

#include "CreatureInspectionComponent.h"
#include "ObservableCreature.h"
#include "ObservationTarget.h"
#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"


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
    if (!Target)
    {
        return;
    }


    APlayerCharacter* Player = GetPlayer();

    if (!Player)
    {
        return;
    }


    Player->GetCharacterMovement()->DisableMovement();

    Player->GetCharacterMovement()->StopMovementImmediately();


    APlayerController* PC =
        Cast<APlayerController>(Player->GetController());


    if (!PC)
    {
        return;
    }


    CurrentTarget = Target;


    PC->bShowMouseCursor = true;


    FInputModeGameAndUI Mode;

    Mode.SetHideCursorDuringCapture(false);

    Mode.SetLockMouseToViewportBehavior(
        EMouseLockMode::DoNotLock);


    PC->SetInputMode(Mode);


    if (Player->CreatureInspectionComponent)
    {
        Player->CreatureInspectionComponent
            ->SetObservedCreature(
                Cast<AObservableCreature>(Target));
    }
}


void UObservationComponent::EndObservation()
{
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


    if (Player->CreatureInspectionComponent)
    {
        Player->CreatureInspectionComponent
            ->ClearObservedCreature();
    }

    CurrentTarget = nullptr;

    Player->GetCharacterMovement()
        ->SetMovementMode(MOVE_Walking);


    PC->SetInputMode(
        FInputModeGameOnly());


    PC->bShowMouseCursor = false;
}


void UObservationComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction);


    if (!CurrentTarget)
    {
        return;
    }


    UpdateCursorTrace();

    UpdateOrbitCamera();
}


void UObservationComponent::UpdateCursorTrace()
{
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


    if (PC->GetHitResultUnderCursor(
        ECC_Visibility,
        false,
        Hit))
    {
        if (Hit.GetActor())
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

void UObservationComponent::UpdateOrbitCamera()
{
    AObservableCreature* Creature =
        Cast<AObservableCreature>(
            CurrentTarget);

    if (!Creature)
    {
        return;
    }

    USpringArmComponent* CameraBoom =
        Creature->GetCameraBoom();

    if (!CameraBoom)
    {
        return;
    }

    CameraBoom->SetRelativeRotation(
        FRotator(
            OrbitPitch,
            OrbitYaw,
            0.f));
}

void UObservationComponent::AddOrbitInput(
    float YawInput,
    float PitchInput)
{
    OrbitYaw += YawInput * OrbitSensitivity;

    OrbitPitch =
        FMath::Clamp(
            OrbitPitch + PitchInput * OrbitSensitivity,
            -60.f,
            60.f);
}
