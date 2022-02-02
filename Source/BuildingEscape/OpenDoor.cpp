// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#define OUT


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

	DoorAudio = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!DoorAudio)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s actor doesn't have audio component"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentYaw = GetOwner()->GetActorRotation().Yaw;

	if (TotalMassOfActors() >= MassToOpen)
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
	DoorAudioPlay(false);
}

void UOpenDoor::CloseDoor(float DTime)
{
	FRotator ClosedDoor(0.f, ClosedYaw, 0.f);
	ClosedDoor.Yaw = FMath::FInterpTo(CurrentYaw, ClosedYaw, DTime, ClosingSpeed);
	GetOwner()->SetActorRotation(ClosedDoor);
	DoorAudioPlay(true);
}

void UOpenDoor::DoorAudioPlay(bool IsClosing)
{	
	if (!DoorAudio)
	{
		UE_LOG
		(
			LogTemp, Error, 
			TEXT("%s actor can't play any audio because of missing audio component"), 
			*GetOwner()->GetName()
		);
		return;
	}

	if (IsClosing && !ClosingAudioPlayed)
	{
		DoorAudio->Play();
		ClosingAudioPlayed = true;
		OpeningAudioPlayed = false;
	}
	else if (!IsClosing && !OpeningAudioPlayed)
	{
		DoorAudio->Play();
		OpeningAudioPlayed = true;
		ClosingAudioPlayed = false;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component with PressurePlate == NULL"), *GetOwner()->GetName());
		return TotalMass;
	}

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}