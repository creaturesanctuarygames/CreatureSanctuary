#include "ObservableCreature.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "PlayerCharacter.h"
#include "InteractableComponent.h"
#include "InteractionComponent.h"

AObservableCreature::AObservableCreature()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh =
		CreateDefaultSubobject<UStaticMeshComponent>(
			TEXT("Mesh"));

	RootComponent = Mesh;

	InteractionSphere =
		CreateDefaultSubobject<USphereComponent>(
			TEXT("InteractionSphere"));

	InteractionSphere->SetupAttachment(RootComponent);

	InteractionSphere->SetSphereRadius(300.f);

	InteractableComponent =
		CreateDefaultSubobject<UInteractableComponent>(
			TEXT("InteractableComponent"));

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(
		this,
		&AObservableCreature::OnInteractionSphereBeginOverlap);

	InteractionSphere->OnComponentEndOverlap.AddDynamic(
		this,
		&AObservableCreature::OnInteractionSphereEndOverlap);

	InteractionSphere->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly);

	InteractionSphere->SetCollisionResponseToAllChannels(
		ECR_Ignore);

	InteractionSphere->SetCollisionResponseToChannel(
		ECC_Pawn,
		ECR_Overlap);

	ObservationPoint =
		CreateDefaultSubobject<USceneComponent>(
			TEXT("ObservationPoint"));

	ObservationPoint->SetupAttachment(RootComponent);
}

void AObservableCreature::OnInteractionSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerCharacter* Player =
		Cast<APlayerCharacter>(OtherActor);

	if (!Player)
	{
		return;
	}

	if (!InteractableComponent)
	{
		return;
	}

	if (!Player->InteractionComponent)
	{
		return;
	}

	Player->InteractionComponent->RegisterInteractable(InteractableComponent);
}

void AObservableCreature::OnInteractionSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	APlayerCharacter* Player =
		Cast<APlayerCharacter>(OtherActor);

	UE_LOG(LogTemp, Warning,
		TEXT("Overlap End"));

	if (!Player)
	{
		return;
	}

	if (!InteractableComponent)
	{
		return;
	}

	if (!Player->InteractionComponent)
	{
		return;
	}

	Player->InteractionComponent->UnregisterInteractable(InteractableComponent);
}

void AObservableCreature::Interact(AActor* InteractingActor)
{
	APlayerCharacter* Player =
		Cast<APlayerCharacter>(InteractingActor);

	if (!Player)
	{
		return;
	}

	Player->StartObservation(this);
}

FTransform AObservableCreature::GetObservationTransform() const
{
	if (ObservationPoint)
	{
		return ObservationPoint->GetComponentTransform();
	}

	return GetActorTransform();
}

USphereComponent* AObservableCreature::GetInjuryPoint(ECreatureBodyPart BodyPart) const
{
	const FName DesiredTag = CreatureBodyPart::ToTag(BodyPart);

	TArray<USphereComponent*> Spheres;
	GetComponents<USphereComponent>(Spheres);

	for (USphereComponent* Sphere : Spheres)
	{
		if (!Sphere)
		{
			continue;
		}

		if (Sphere->ComponentHasTag(DesiredTag))
		{
			return Sphere;
		}
	}

	return nullptr;
}

const FCreatureInjury* AObservableCreature::GetInjury(ECreatureBodyPart BodyPart) const
{
	for (const FCreatureInjury& Injury : ActiveInjuries)
	{
		if (Injury.BodyPart == BodyPart)
		{
			return &Injury;
		}
	}

	return nullptr;
}

bool AObservableCreature::RemoveInjury(ECreatureBodyPart BodyPart)
{
	const int32 Removed =
		ActiveInjuries.RemoveAll(
			[BodyPart](const FCreatureInjury& Injury)
			{
				return Injury.BodyPart == BodyPart;
			});

	return Removed > 0;
}
