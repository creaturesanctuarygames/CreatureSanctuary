#include "InteractableComponent.h"

#include "Interactable.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::Interact(AActor* InteractingActor)
{
	IInteractable* Target =
		Cast<IInteractable>(GetOwner());

	if (!Target)
	{
		return;
	}

	Target->Interact(InteractingActor);
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
