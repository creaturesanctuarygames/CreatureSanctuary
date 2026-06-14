#include "ObservableCreature.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "ObservableComponent.h"

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

	ObservableComponent =
		CreateDefaultSubobject<UObservableComponent>(
			TEXT("ObservableComponent"));

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

	if (ObservableComponent)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Overlap Begin"));

		ObservableComponent->RegisterPlayer(Player);
	}
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

	if (ObservableComponent)
	{
		ObservableComponent->UnregisterPlayer(Player);
	}
}

FTransform AObservableCreature::GetObservationTransform() const
{
	if (ObservationPoint)
	{
		return ObservationPoint->GetComponentTransform();
	}

	return GetActorTransform();
}
