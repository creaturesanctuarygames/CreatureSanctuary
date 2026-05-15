#include "PlayerCharacterMovementComponent.h"

UPlayerCharacterMovementComponent::UPlayerCharacterMovementComponent()
{
	// Apply custom tuning to base UE movement system
	MaxAcceleration = MoveAcceleration;
	BrakingDecelerationWalking = MoveBrakingDeceleration;

	GroundFriction = 6.f;
	BrakingFrictionFactor = 1.f;

	AirControl = 0.35f;
}

float UPlayerCharacterMovementComponent::GetMaxSpeed() const
{
	switch (CurrentMovementState)
	{
	case EPlayerMovementState::Sprinting:
		return SprintSpeed;

	default:
		return WalkSpeed;
	}
}

void UPlayerCharacterMovementComponent::OnMovementUpdated(
	float DeltaSeconds,
	const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	UpdateMovementState();
}

void UPlayerCharacterMovementComponent::UpdateMovementState()
{
	// Falling overrides everything
	if (IsFalling())
	{
		CurrentMovementState = EPlayerMovementState::Falling;
		return;
	}

	// Ground states
	if (bWantsToSprint)
	{
		CurrentMovementState = EPlayerMovementState::Sprinting;
	}
	else
	{
		CurrentMovementState = EPlayerMovementState::Walking;
	}
}

void UPlayerCharacterMovementComponent::StartSprinting()
{
	bWantsToSprint = true;
}

void UPlayerCharacterMovementComponent::StopSprinting()
{
	bWantsToSprint = false;
}