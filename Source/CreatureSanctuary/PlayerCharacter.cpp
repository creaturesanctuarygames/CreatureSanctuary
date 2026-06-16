#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "ObservationComponent.h"
#include "ObservableCreature.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	InteractionComponent =
		CreateDefaultSubobject<UInteractionComponent>(
			TEXT("InteractionComponent"));

	ObservationComponent =
		CreateDefaultSubobject<UObservationComponent>(
			TEXT("ObservationComponent"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	if (ObservationCamera)
	{
		ObservationCamera->Deactivate();
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Look
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Move
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Jump (START)
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);

		// Jump (STOP)
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Interact
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);

		// Exit
		EnhancedInput->BindAction(ExitAction, ETriggerEvent::Started, this, &APlayerCharacter::Exit);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	if (bIsObserving)
	{
		OrbitYaw += LookAxis.X;
		OrbitPitch = FMath::Clamp(OrbitPitch + LookAxis.Y, -60.f, 60.f);
		return;
	}

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(-LookAxis.Y);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	// Get controller rotation (yaw only)
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// Get forward/right vectors from controller orientation
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Input.Y);
	AddMovementInput(Right, Input.X);
}

void APlayerCharacter::Interact()
{
	UE_LOG(LogTemp, Warning,
		TEXT("E Pressed"));

	if (InteractionComponent)
	{
		InteractionComponent->Interact();
	}
}

void APlayerCharacter::StartObservation(AActor* Target)
{
	if (!ObservationComponent || !Target)
	{
		return;
	}

	ObservationComponent->BeginObservation(Target);

	AObservableCreature* Creature =
		Cast<AObservableCreature>(Target);

	if (!Creature)
	{
		return;
	}

	if (UCameraComponent* Cam = FindComponentByClass<UCameraComponent>())
	{
		SavedCamLocation = Cam->GetComponentLocation();
		SavedCamRotation = Cam->GetComponentRotation();
	}

	OrbitTarget = Creature->GetObservationTransform().GetLocation();

	OrbitYaw = 0.f;
	OrbitPitch = 10.f;

	bIsObserving = true;
	bIsReturningFromObservation = false;

	APlayerController* PC =
		Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
	}
}

void APlayerCharacter::Exit()
{
	if (ObservationComponent)
	{
		ObservationComponent->EndObservation();
	}

	bIsObserving = false;
	bIsReturningFromObservation = true;

	APlayerController* PC =
		Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		PC->SetControlRotation(GetActorRotation());
	}

	OrbitYaw = 0.f;
	OrbitPitch = 0.f;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCameraComponent* Cam = FindComponentByClass<UCameraComponent>();
	if (!Cam)
	{
		return;
	}

	// =========================
	// OBSERVATION ORBIT MODE
	// =========================
	if (bIsObserving)
	{
		FVector Offset =
			FRotationMatrix(FRotator(OrbitPitch, OrbitYaw, 0.f))
			.GetUnitAxis(EAxis::X) * -OrbitDistance;

		FVector DesiredLocation = OrbitTarget + Offset;

		FRotator LookAt = (OrbitTarget - DesiredLocation).Rotation();

		Cam->SetWorldLocation(FMath::VInterpTo(
			Cam->GetComponentLocation(),
			DesiredLocation,
			DeltaTime,
			6.f));

		Cam->SetWorldRotation(FMath::RInterpTo(
			Cam->GetComponentRotation(),
			LookAt,
			DeltaTime,
			6.f));

		return;
	}

	// =========================
	// RETURN TO PLAYER CAMERA
	// =========================
	if (bIsReturningFromObservation)
	{
		FVector NewLoc = FMath::VInterpTo(
			Cam->GetComponentLocation(),
			SavedCamLocation,
			DeltaTime,
			ReturnInterpSpeed);

		FRotator NewRot = FMath::RInterpTo(
			Cam->GetComponentRotation(),
			SavedCamRotation,
			DeltaTime,
			ReturnInterpSpeed);

		Cam->SetWorldLocation(NewLoc);
		Cam->SetWorldRotation(NewRot);

		if (FVector::Dist(NewLoc, SavedCamLocation) < 2.f)
		{
			bIsReturningFromObservation = false;

			OrbitYaw = 0.f;
			OrbitPitch = 0.f;
			OrbitTarget = FVector::ZeroVector;

			APlayerController* PC =
				Cast<APlayerController>(GetController());

			if (PC)
			{
				PC->SetControlRotation(GetActorRotation());
			}
		}

		return;
	}
}
