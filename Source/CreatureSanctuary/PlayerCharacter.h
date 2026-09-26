#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UInteractionComponent;
class UObservationComponent;
class UCreatureInspectionComponent;

struct FInputActionValue;

UCLASS()
class CREATURESANCTUARY_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:

    APlayerCharacter();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    void StartObservation(AActor* Target);

    void OnCameraTransitionFinished();

public:

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UInteractionComponent> InteractionComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UObservationComponent> ObservationComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCreatureInspectionComponent> CreatureInspectionComponent;

protected:

    void Move(const FInputActionValue& Value);

    void Look(const FInputActionValue& Value);

    void Interact();

    void Exit();

protected:

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> ExitAction;

private:

    UPROPERTY()
    bool bIsObserving = false;

    UPROPERTY()
    bool bIsCameraTransitioning = false;

    FTimerHandle CameraTransitionTimer;
};