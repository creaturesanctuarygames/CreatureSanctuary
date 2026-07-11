#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UInteractableComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	virtual void BeginPlay() override;
	virtual void OnRegister() override;

	void RegisterInteractable(UInteractableComponent* Interactable);
	void UnregisterInteractable(UInteractableComponent* Interactable);

	void Interact();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;


private:
	void UpdateCurrentInteractable();
	
private:
	UPROPERTY()
	TArray<UInteractableComponent*> NearbyInteractables;

	UPROPERTY()
	UInteractableComponent* CurrentInteractable = nullptr;

	UInteractableComponent* LastHighlighted = nullptr;
};