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
}


void UOpenDoor::OpenDoor()
{
	float yawCurrent = Owner->GetActorRotation().Yaw;
	Owner->SetActorRotation(FRotator(0.f, yawCurrent + OpenAngle, 0.f));
	bIsDoorOpen = true;
}


void UOpenDoor::CloseDoor()
{
	float yawCurrent = Owner->GetActorRotation().Yaw;
	Owner->SetActorRotation(FRotator(0.f, yawCurrent - OpenAngle, 0.f));
	bIsDoorOpen = false;
} 

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (PressurePlate && GetTotalMassOfActorsOnPlate() > 50.f) // TODO parameter
	{
		if (!bIsDoorOpen)
		{
			OpenDoor();
		}
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (LastDoorOpenTime != 0 && (GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
	{
		if (bIsDoorOpen)
		{
			CloseDoor();
		}
		LastDoorOpenTime = 0;
	}
}


float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
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
