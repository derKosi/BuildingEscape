// Copyright Mathias Kosinski 2017

#include "OpenDoor.h"

#include "BuildingEscape.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *(GetOwner()->GetName()));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PressurePlate) return;
	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() >= TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}


float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) return 0.f;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their mass
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *(Actor->GetName()));
	}
	//FString TheFloatStr = FString::SanitizeFloat(TotalMass);
	//UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *TheFloatStr);

	return TotalMass;
}
