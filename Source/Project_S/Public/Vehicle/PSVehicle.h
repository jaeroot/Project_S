// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_S.h"
#include "GameFramework/Pawn.h"
#include "PSVehicle.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECT_S_API APSVehicle : public APawn
{
	GENERATED_BODY()

public:
	APSVehicle();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void UnPossessed() override;

private:
	void UpdateVehicleSuspension(int WheelIndex, float DeltaTime);
	void UpdateVehicleForce();
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	
	void HandbrakePressed();
	void HandbrakeReleased();
	
	void PossessCharacter();

public:
	UPROPERTY(VisibleAnywhere, Category = "Vehicle")
	UStaticMeshComponent* VehicleMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UArrowComponent* Arrow_FR;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UArrowComponent* Arrow_FL;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UArrowComponent* Arrow_RR;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UArrowComponent* Arrow_RL;
	
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	USceneComponent* WheelScene_FR;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	USceneComponent* WheelScene_FL;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	USceneComponent* WheelScene_RR;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	USceneComponent* WheelScene_RL;
	
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UStaticMeshComponent* Wheel_FR;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UStaticMeshComponent* Wheel_FL;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UStaticMeshComponent* Wheel_RR;
	UPROPERTY(VisibleAnywhere, Category = "Wheel")
	UStaticMeshComponent* Wheel_RL;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float MaxWheelNum = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float RestLength = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float SpringLength = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float WheelRadius = 31.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float SpringForceConst = 50000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float DamperForceConst = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float ForwardForceConst = 100000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float MaxSteeringAngle = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float FrictionConst = 500;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float BrakeConst = 1000;

	class APSCharacter* Character;
	
protected:
	enum class EWheelIndex
	{
		FR,
		FL,
		RR,
		RL
	};

private:
	TArray<UArrowComponent*> WheelArrowComponentHolder;
	TArray<USceneComponent*> WheelSceneComponentHolder;
	TArray<UStaticMeshComponent*> WheelMeshComponentHolder;
	FCollisionQueryParams LineTraceCollisionQuery;

	float MinLength;
	float MaxLength;
	float ForwardAxisValue = 0;
	float RightAxisValue = 0;
	bool bBrakeApplied = false;

};
