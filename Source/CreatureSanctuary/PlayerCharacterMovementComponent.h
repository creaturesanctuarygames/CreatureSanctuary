#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	Walking,
	Sprinting,
	Falling
};

UCLASS()
class CREATURESANCTUARY_API UPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPlayerCharacterMovementComponent();

	virtual float GetMaxSpeed() const override;

	virtual void OnMovementUpdated(
		float DeltaSeconds,
		const FVector& OldLocation,
		const FVector& OldVelocity) override;

	UFUNCTION(BlueprintCallable)
	void StartSprinting();

	UFUNCTION(BlueprintCallable)
	void StopSprinting();

	UFUNCTION(BlueprintPure)
	EPlayerMovementState GetMovementState() const { return CurrentMovementState; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 650.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SpeedInterpRate = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveAcceleration = 2048.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveBrakingDeceleration = 2048.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPlayerMovementState CurrentMovementState = EPlayerMovementState::Walking;

private:

	bool bWantsToSprint = false;

	float CurrentSpeed = 0.f;

	void UpdateMovementState();
};