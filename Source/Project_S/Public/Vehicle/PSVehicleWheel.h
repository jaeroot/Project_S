// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSVehicleWheel.generated.h"



UENUM()
enum class EWheelPosition : uint8
{
	Undefined,
	Front,
	Rear
};

UCLASS()
class PROJECT_S_API APSVehicleWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	APSVehicleWheel();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	
public:	
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	class UArrowComponent* WheelArrow;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	USceneComponent* WheelScene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wheel")
	UStaticMeshComponent* WheelMesh;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wheel")
	float WheelRadius = 31.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wheel")
	EWheelPosition WheelPos = EWheelPosition::Undefined;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	float RestLength = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	float SpringLength = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	float SpringForceConst = 50000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	float DamperForceConst = 5000;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering")
	float MaxSteeringAngle = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Steering")
	bool bSteering = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brake")
	float BrakeConst = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brake")
	bool bBrake = true;

protected:

private:
	
};
