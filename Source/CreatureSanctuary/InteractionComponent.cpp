#include "InteractionComponent.h"
#include "ObservableComponent.h"

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

void UInteractionComponent::RegisterObservable(
	UObservableComponent* Observable)
{
	NearbyObservables.AddUnique(Observable);

	UpdateCurrentObservable();
}

void UInteractionComponent::UnregisterObservable(
	UObservableComponent* Observable)
{
	NearbyObservables.Remove(Observable);

	UpdateCurrentObservable();
}

void UInteractionComponent::Interact()
{
	if (NearbyObservables.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing nearby"));
		return;
	}

	if (!CurrentObservable)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Nothing nearby"));

		return;
	}

	CurrentObservable->Interact();
}

void UInteractionComponent::UpdateCurrentObservable()
{
	CurrentObservable = nullptr;

	AActor* Owner = GetOwner();

	if (!Owner)
	{
		return;
	}

	float ClosestDistanceSq = MAX_FLT;

	for (UObservableComponent* Observable : NearbyObservables)
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
			CurrentObservable = Observable;
		}
	}
}

void UInteractionComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (NearbyObservables.Num() == 0)
	{
		if (LastHighlighted)
		{
			LastHighlighted->SetHighlighted(false);
			LastHighlighted = nullptr;
		}

		return;
	}

	UObservableComponent* Previous = CurrentObservable;

	UpdateCurrentObservable();

	if (LastHighlighted != CurrentObservable)
	{
		if (LastHighlighted)
		{
			LastHighlighted->SetHighlighted(false);
		}

		if (CurrentObservable)
		{
			CurrentObservable->SetHighlighted(true);
		}

		UE_LOG(LogTemp, Warning,
			TEXT("Highlight switch: %s  %s"),
			*GetNameSafe(LastHighlighted ? LastHighlighted->GetOwner() : nullptr),
			*GetNameSafe(CurrentObservable ? CurrentObservable->GetOwner() : nullptr));

		LastHighlighted = CurrentObservable;



	}
}
