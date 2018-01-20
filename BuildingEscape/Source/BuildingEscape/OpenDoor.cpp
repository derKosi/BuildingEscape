// Copyright Mathias Kosinski 2017

#include "OpenDoor.h"

#include "BuildingEscape.h"
#include "Engine/World.h"
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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


void UOpenDoor::OpenDoor()
{
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	float yRotation = Owner->GetActorRotation().Yaw;
	Owner->SetActorRotation(FRotator(0.f, yRotation + OpenAngle, 0.f));
	bIsDoorOpen = true;
}


void UOpenDoor::CloseDoor()
{
	//Owner->SetActorRotation(FRotator(0.f, 180.f, 0.f));
	float yRotation = Owner->GetActorRotation().Yaw;
	Owner->SetActorRotation(FRotator(0.f, yRotation - OpenAngle, 0.f));
	bIsDoorOpen = false;
} 

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume

	// If the ActorThatOpens is in the volume
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		if (!bIsDoorOpen)
		{
			OpenDoor();
		}
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
	{
		if (bIsDoorOpen)
		{
			CloseDoor();
		}
		LastDoorOpenTime = 0;
	}
}

