#pragma once

#include "CoreMinimal.h"
#include "ObservationTarget.h"
#include "Interactable.h"
#include "CreatureInjury.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ObservableCreature.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UInteractableComponent;
class USceneComponent;

UCLASS()
class CREATURESANCTUARY_API AObservableCreature : public AActor, public IInteractable, public IObservationTarget
{
	GENERATED_BODY()

public:
	AObservableCreature();

	UFUNCTION()
	void OnInteractionSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void Interact(AActor* InteractingActor) override;

	FTransform GetObservationTransform() const;
	USphereComponent* GetInjuryPoint(ECreatureBodyPart BodyPart) const;

	bool RemoveInjury(ECreatureBodyPart BodyPart);

	const FCreatureInjury* GetInjury(ECreatureBodyPart BodyPart) const;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere)
	UInteractableComponent* InteractableComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ObservationPoint;

	UPROPERTY(EditAnywhere)
	FVector ObservationCameraOffset = FVector(-150, 0, 80);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Injuries")
	TArray<FCreatureInjury> ActiveInjuries;
};