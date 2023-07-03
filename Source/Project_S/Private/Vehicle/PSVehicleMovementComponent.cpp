// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/PSVehicleMovementComponent.h"

#include "Vehicle/PSVehicleWheel.h"


FVehicleWheel::FVehicleWheel()
	: WheelType(APSVehicleWheel::StaticClass())
	, SocketName(NAME_None)
{
}

UPSVehicleMovementComponent::UPSVehicleMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	

}

void UPSVehicleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPSVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}