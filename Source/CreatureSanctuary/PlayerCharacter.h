#pragma once

#include <Camera/CameraComponent.h>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UInteractionComponent;
class UObservationComponent;
class AObservableCreature;

struct FInputActionValue;

UCLASS()
class CREATURESANCTUARY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UObservationComponent> ObservationComponent;

	APlayerCharacter();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	void StartObservation(AActor* Target);

protected:

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	void Interact();

	void Exit();

	UPROPERTY()
	UCameraComponent* ObservationCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ExitAction;

	FVector SavedCamLocation;
	FRotator SavedCamRotation;

	bool bIsReturningFromObservation = false;
	float ReturnInterpSpeed = 6.f;

	FVector ObservationCamTargetLocation;
	FRotator ObservationCamTargetRotation;

	bool bIsInObservationTransition = false;

	float ObservationInterpSpeed = 6.f;

	UPROPERTY()
	bool bIsObserving = false;

	UPROPERTY()
	FVector OrbitTarget;

	UPROPERTY()
	float OrbitDistance = 200.f;

	UPROPERTY()
	float OrbitYaw = 0.f;

	UPROPERTY()
	float OrbitPitch = 10.f;

	UPROPERTY()
	float OrbitSpeed = 80.f;
};