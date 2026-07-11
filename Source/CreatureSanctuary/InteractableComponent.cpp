#include "InteractableComponent.h"

#include "ObservationTarget.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::Interact(AActor* InteractingActor)
{
	IObservationTarget* Target =
		Cast<IObservationTarget>(GetOwner());

	if (!Target)
	{
		return;
	}

	Target->OnInteract(InteractingActor);
}

void UInteractableComponent::SetHighlighted(bool bHighlighted)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	TArray<UMeshComponent*> Meshes;
	Owner->GetComponents<UMeshComponent>(Meshes);

	for (UMeshComponent* Mesh : Meshes)
	{
		if (!Mesh) continue;

		Mesh->SetRenderCustomDepth(bHighlighted);
		Mesh->SetCustomDepthStencilValue(1);
	}
}
