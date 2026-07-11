#include "InteractionComponent.h"
#include "InteractableComponent.h"
#include "PlayerCharacter.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UInteractionComponent::OnRegister()
{
	Super::OnRegister();
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

	PrimaryComponentTick.RegisterTickFunction(GetWorld()->PersistentLevel);
}

void UInteractionComponent::RegisterInteractable(
	UInteractableComponent* Interactable)
{
	NearbyInteractables.AddUnique(Interactable);

	UpdateCurrentInteractable();
}

void UInteractionComponent::UnregisterInteractable(
	UInteractableComponent* Interactable)
{
	NearbyInteractables.Remove(Interactable);

	UpdateCurrentInteractable();
}

void UInteractionComponent::Interact()
{
	if (NearbyInteractables.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing nearby"));
		return;
	}

	UpdateCurrentInteractable();

	if (!CurrentInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT("No current observable"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Interacting with: %s"),
		*GetNameSafe(CurrentInteractable->GetOwner()));

	CurrentInteractable->Interact(GetOwner());
}

void UInteractionComponent::UpdateCurrentInteractable()
{
	CurrentInteractable = nullptr;

	AActor* Owner = GetOwner();

	if (!Owner)
	{
		return;
	}

	float ClosestDistanceSq = MAX_FLT;

	for (UInteractableComponent* Observable : NearbyInteractables)
	{
		if (!Observable)
		{
			continue;
		}

		AActor* ObservableActor = Observable->GetOwner();

		if (!ObservableActor)
		{
			continue;
		}

		const float DistanceSq =
			FVector::DistSquared(
				Owner->GetActorLocation(),
				ObservableActor->GetActorLocation());

		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistanceSq;
			CurrentInteractable = Observable;
		}
	}
}

void UInteractionComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (NearbyInteractables.Num() == 0)
	{
		if (LastHighlighted)
		{
			LastHighlighted->SetHighlighted(false);
			LastHighlighted = nullptr;
		}

		return;
	}

	UInteractableComponent* Previous = CurrentInteractable;

	UpdateCurrentInteractable();

	if (LastHighlighted != CurrentInteractable)
	{
		if (LastHighlighted)
		{
			LastHighlighted->SetHighlighted(false);
		}

		if (CurrentInteractable)
		{
			CurrentInteractable->SetHighlighted(true);
		}

		UE_LOG(LogTemp, Warning,
			TEXT("Highlight switch: %s  %s"),
			*GetNameSafe(LastHighlighted ? LastHighlighted->GetOwner() : nullptr),
			*GetNameSafe(CurrentInteractable ? CurrentInteractable->GetOwner() : nullptr));

		LastHighlighted = CurrentInteractable;



	}
}
