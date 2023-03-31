// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_S.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "PSCharacter.generated.h"

UCLASS()
class PROJECT_S_API APSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APSCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;
	void Attack();
	ECharacterMotion GetCurrentCharacterMotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetCharacterMotion(ECharacterMotion NewCharacterMotion);

private:
	void BlockControlRotation();
	void ReleaseControlRotation();
	void DoCrouch();
	void DoProne();
	void Run();
	void StopRun();
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	FOnTimelineFloat CrouchTimelineFunction;
	FOnTimelineFloat ProneTimelineFunction;
	FOnTimelineFloat CameraRotTimelineFunction;
	FOnTimelineEvent CameraRotTimelineFinish;

	UFUNCTION()
		void CrouchInterp(float Value);
	
	UFUNCTION()
		void ProneInterp(float Value);

	UFUNCTION()
		void CameraRotInterp(float Value);

	UFUNCTION()
		void CameraRotFinish();

	bool CanStand(float HalfHeight);


public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		UCurveFloat* CrouchCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		UCurveFloat* ProneCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		UCurveFloat* CameraRotCurve;

	bool Running;

protected:
	ECharacterMotion CurrentCharacterMotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
		UTimelineComponent* CrouchTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
		UTimelineComponent* ProneTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timeline")
		UTimelineComponent* CameraRotTimeline;

private:
	bool ControlRotationBlocked;
	FRotator ControlRotation;

	float StandHalfHeight;
	float CrouchHalfHeight;
	float ProneHalfHeight;
	float Radius;

	bool RunInput;
	bool MovingBack;
	bool MovingSide;
};
