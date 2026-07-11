#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();

	void Interact(APlayerCharacter* Player);

	void RegisterPlayer(APlayerCharacter* Player);
	void UnregisterPlayer(APlayerCharacter* Player);

	void SetHighlighted(bool bHighlighted);
};