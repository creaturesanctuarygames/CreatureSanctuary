#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UObservableComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	virtual void BeginPlay() override;
	virtual void OnRegister() override;

	void RegisterObservable(UObservableComponent* Observable);
	void UnregisterObservable(UObservableComponent* Observable);

	void Interact();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;


private:
	void UpdateCurrentObservable();

private:
	UPROPERTY()
	TArray<UObservableComponent*> NearbyObservables;

	UPROPERTY()
	UObservableComponent* CurrentObservable = nullptr;

	UObservableComponent* LastHighlighted = nullptr;
};