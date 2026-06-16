#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservableComponent.generated.h"

class APlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATURESANCTUARY_API UObservableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObservableComponent();

	void Interact(APlayerCharacter* Player);

	void RegisterPlayer(APlayerCharacter* Player);
	void UnregisterPlayer(APlayerCharacter* Player);

	void SetHighlighted(bool bHighlighted);
};