// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Project_S.h"
#include "GameFramework/Character.h"
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
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);


public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;


protected:
	ECharacterMotion CurrentCharacterMotion;

private:
	bool ControlRotationBlocked;
};
