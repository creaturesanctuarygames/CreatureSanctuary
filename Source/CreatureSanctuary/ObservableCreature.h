#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObservableCreature.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UObservableComponent;

UCLASS()
class CREATURESANCTUARY_API AObservableCreature : public AActor
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

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere)
	UObservableComponent* ObservableComponent;
};