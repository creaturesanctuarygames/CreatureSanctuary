#include "ObservableComponent.h"

#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"
#include "InteractionComponent.h"

UObservableComponent::UObservableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObservableComponent::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Observed Creature"));
}

void UObservableComponent::RegisterPlayer(APlayerCharacter* Player)
{
	if (!Player)
	{
		return;
	}

	if (Player->InteractionComponent)
	{
		Player->InteractionComponent->RegisterObservable(this);
	}
}

void UObservableComponent::UnregisterPlayer(APlayerCharacter* Player)
{
	if (!Player)
	{
		return;
	}

	if (Player->InteractionComponent)
	{
		Player->InteractionComponent->UnregisterObservable(this);
	}
}

void UObservableComponent::SetHighlighted(bool bHighlighted)
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
