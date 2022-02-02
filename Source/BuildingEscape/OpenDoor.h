// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DTime);
	void CloseDoor(float DTime);
	float TotalMassOfActors() const;
	void DoorAudioPlay(bool IsClosing);

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY()
	UAudioComponent* DoorAudio = nullptr;

	UPROPERTY(EditAnywhere)
	float InitialYaw = 90.f;
	float CurrentYaw;
	float ClosedYaw;
	float OpenedYaw;

	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float OpeningSpeed = 2.f;
	UPROPERTY(EditAnywhere)
	float ClosingSpeed = 5.f;
	UPROPERTY(EditAnywhere)
	float MassToOpen = 50.f;

	bool OpeningAudioPlayed = false;
	bool ClosingAudioPlayed = true;
};
