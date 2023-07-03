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
	APSCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Jump() override;
	void Attack();
	ECharacterMotion GetCurrentCharacterMotion();

protected:
	virtual void BeginPlay() override;

	void SetCharacterMotion(ECharacterMotion NewCharacterMotion);

private:
	void BlockControlRotation();
	void ReleaseControlRotation();
	void DoCrouch();
	void DoProne();
	void RunButtonPressed();
	void RunButtonReleased();
	void PossessVehicle();
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	FOnTimelineFloat CrouchTimelineFunction;
	FOnTimelineEvent CrouchTimelineFinish;

	FOnTimelineFloat ProneTimelineFunction;
	FOnTimelineEvent ProneTimelineFinish;

	FOnTimelineFloat CameraRotTimelineFunction;
	FOnTimelineEvent CameraRotTimelineFinish;

	UFUNCTION()
		void CrouchInterp(float Value);

	UFUNCTION()
		void CrouchFinish();
	
	UFUNCTION()
		void ProneInterp(float Value);

	UFUNCTION()
		void ProneFinish();

	UFUNCTION()
		void CameraRotInterp(float Value);

	UFUNCTION()
		void CameraRotFinish();

	bool CanStand(float HalfHeight);

	void SetRunInput(bool bIsRunInput);
	UFUNCTION(Server, Reliable)
	void ServerSetRunInput(bool bIsRunInput);

	void SetRun(bool bIsRun);
	UFUNCTION(Server, Reliable)
	void ServerSetRun(bool bIsRun);

public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		UCurveFloat* CrouchCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		UCurveFloat* ProneCurve;

	UPROPERTY(VisibleAnywhere, Category = "Timeline")
		UCurveFloat* CameraRotCurve;

	UPROPERTY(Replicated)
	bool Running;

protected:
	UPROPERTY(Replicated)
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

	UPROPERTY(Replicated)
	bool RunInput;
	bool MovingBack;
	bool MovingSide;

	bool IsCrouch;
	bool IsProne;
};
