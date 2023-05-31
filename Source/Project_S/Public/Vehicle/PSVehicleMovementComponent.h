// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PSVehicleMovementComponent.generated.h"

class APSVehicleWheel;

USTRUCT()
struct FVehicleWheel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category = "VehicleWheel")
	TSubclassOf<APSVehicleWheel> WheelType;

	UPROPERTY(EditAnywhere, Category = "VehicleWheel")
	FName SocketName;

	FVehicleWheel();
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_S_API UPSVehicleMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPSVehicleMovementComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateVehicleMovement();
	
protected:
	virtual void BeginPlay() override;

private:
	void UpdateVehicleSuspension();
	void UpdateVehicleSteering();
	void UpdateVehicleForce();

public:
	UPROPERTY(EditAnywhere, Category = "VehicleWheel")
	TArray<FVehicleWheel> VehicleWheels;

protected:

private:
	float MinSpringLength;
	float MaxSpringLength;
};
