// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


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

	ClosedYaw = GetOwner()->GetActorRotation().Yaw;
	OpenedYaw = ClosedYaw + InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component with PressurePlate == NULL"), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentYaw = GetOwner()->GetActorRotation().Yaw;

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() >= DoorLastOpened + DoorCloseDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DTime)
{
	FRotator OpenedDoor(0.f, OpenedYaw, 0.f);
	OpenedDoor.Yaw = FMath::FInterpTo(CurrentYaw, OpenedYaw, DTime, OpeningSpeed);
	GetOwner()->SetActorRotation(OpenedDoor);
}

void UOpenDoor::CloseDoor(float DTime)
{
	FRotator ClosedDoor(0.f, ClosedYaw, 0.f);
	ClosedDoor.Yaw = FMath::FInterpTo(CurrentYaw, ClosedYaw, DTime, ClosingSpeed);
	GetOwner()->SetActorRotation(ClosedDoor);
}