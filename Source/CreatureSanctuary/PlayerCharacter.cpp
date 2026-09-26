#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InteractionComponent.h"
#include "ObservationComponent.h"
#include "CreatureInspectionComponent.h"

#include "InputActionValue.h"

#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ObservableCreature.h"


APlayerCharacter::APlayerCharacter()
{
    InteractionComponent =
        CreateDefaultSubobject<UInteractionComponent>(
            TEXT("InteractionComponent"));

    ObservationComponent =
        CreateDefaultSubobject<UObservationComponent>(
            TEXT("ObservationComponent"));

    CreatureInspectionComponent =
        CreateDefaultSubobject<UCreatureInspectionComponent>(
            TEXT("CreatureInspectionComponent"));
}


void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC =
        Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
            {
                Subsystem->AddMappingContext(
                    DefaultMappingContext,
                    0);
            }
        }
    }
}


void APlayerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(
        PlayerInputComponent);


    if (UEnhancedInputComponent* EnhancedInput =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        EnhancedInput->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &APlayerCharacter::Look);


        EnhancedInput->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &APlayerCharacter::Move);


        EnhancedInput->BindAction(
            JumpAction,
            ETriggerEvent::Started,
            this,
            &ACharacter::Jump);


        EnhancedInput->BindAction(
            JumpAction,
            ETriggerEvent::Completed,
            this,
            &ACharacter::StopJumping);


        EnhancedInput->BindAction(
            InteractAction,
            ETriggerEvent::Started,
            this,
            &APlayerCharacter::Interact);


        EnhancedInput->BindAction(
            ExitAction,
            ETriggerEvent::Started,
            this,
            &APlayerCharacter::Exit);
    }
}


void APlayerCharacter::Look(
    const FInputActionValue& Value)
{
    const FVector2D LookAxis =
        Value.Get<FVector2D>();


    if (!Controller)
    {
        return;
    }


    if (bIsObserving)
    {
        if (ObservationComponent)
        {
            ObservationComponent->AddOrbitInput(
                LookAxis.X,
                LookAxis.Y);
        }

        return;
    }


    AddControllerYawInput(
        LookAxis.X);

    AddControllerPitchInput(
        -LookAxis.Y);
}


void APlayerCharacter::Move(
    const FInputActionValue& Value)
{
    const FVector2D Input =
        Value.Get<FVector2D>();


    if (!Controller)
    {
        return;
    }


    const FRotator Rotation =
        Controller->GetControlRotation();


    const FRotator YawRotation(
        0.f,
        Rotation.Yaw,
        0.f);


    const FVector Forward =
        FRotationMatrix(YawRotation)
        .GetUnitAxis(EAxis::X);


    const FVector Right =
        FRotationMatrix(YawRotation)
        .GetUnitAxis(EAxis::Y);


    AddMovementInput(
        Forward,
        Input.Y);


    AddMovementInput(
        Right,
        Input.X);
}


void APlayerCharacter::Interact()
{
    if (bIsObserving)
    {
        if (CreatureInspectionComponent)
        {
            CreatureInspectionComponent
                ->TreatHoveredInjury();
        }

        return;
    }


    if (InteractionComponent)
    {
        InteractionComponent->Interact();
    }
}


void APlayerCharacter::StartObservation(
    AActor* Target)
{
    if (!ObservationComponent || !Target || bIsCameraTransitioning)
    {
        return;
    }

    AObservableCreature* Creature =
        Cast<AObservableCreature>(Target);


    if (!Creature)
    {
        return;
    }

    APlayerController* PC =
        Cast<APlayerController>(GetController());

    if (PC)
    {
        ObservationComponent
            ->BeginObservation(Target);

        bIsObserving = true;
        bIsCameraTransitioning = true;

        FViewTargetTransitionParams Params;
        Params.BlendTime = 1.0f;
        Params.BlendFunction = VTBlend_Cubic;

        PC->SetViewTarget(
            Creature,
            Params);

        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);

        GetWorldTimerManager().SetTimer(
            CameraTransitionTimer,
            this,
            &APlayerCharacter::OnCameraTransitionFinished,
            1.0f,
            false);
    }
}


void APlayerCharacter::Exit()
{
    if (!bIsObserving || bIsCameraTransitioning)
    {
        return;
    }


    if (ObservationComponent)
    {
        ObservationComponent->EndObservation();
    }

    bIsObserving = false;

    APlayerController* PC =
        Cast<APlayerController>(GetController());


    if (PC)
    {
        bIsCameraTransitioning = true;

        FViewTargetTransitionParams Params;
        Params.BlendTime = 1.0f;
        Params.BlendFunction = VTBlend_Cubic;

        PC->SetViewTarget(
            this,
            Params);

        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);

        GetWorldTimerManager().SetTimer(
            CameraTransitionTimer,
            this,
            &APlayerCharacter::OnCameraTransitionFinished,
            1.0f,
            false);
    }
}

void APlayerCharacter::OnCameraTransitionFinished()
{
    bIsCameraTransitioning = false;
}

void APlayerCharacter::Tick(
    float DeltaTime)
{
    Super::Tick(
        DeltaTime);
}