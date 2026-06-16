#include "ObservationComponent.h"

#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ObservableTarget.h"
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

    IObservableTarget* Target =
        Cast<IObservableTarget>(CurrentTarget);

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
