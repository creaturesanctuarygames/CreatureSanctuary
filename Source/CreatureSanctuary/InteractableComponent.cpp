#include "InteractableComponent.h"

#include "ObservableTarget.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"
#include "InteractionComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::Interact(APlayerCharacter* Player)
{
	if (!Player)
	{
		return;
	}

	Player->StartObservation(GetOwner());
}

void UInteractableComponent::RegisterPlayer(APlayerCharacter* Player)
{
	if (!Player)
	{
		return;
	}

	if (Player->InteractionComponent)
	{
		Player->InteractionComponent->RegisterInteractable(this);
	}
}

void UInteractableComponent::UnregisterPlayer(APlayerCharacter* Player)
{
	if (!Player)
	{
		return;
	}

	if (Player->InteractionComponent)
	{
		Player->InteractionComponent->UnregisterInteractable(this);
	}
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
