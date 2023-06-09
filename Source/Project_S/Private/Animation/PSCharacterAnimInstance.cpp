// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PSCharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/PSCharacter.h"


UPSCharacterAnimInstance::UPSCharacterAnimInstance()
{
	CurrentPawnVelocity = FVector::ZeroVector;
	CurrentPawnSpeed = 0.0f;
	CurrentPawnDirection = 0.0f;
	IsInAir = false;
	CurrentPawnMotion = ECharacterMotion::Stand;
	IsRunning = false;
}


void UPSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	CurrentPawnVelocity = Pawn->GetVelocity();
	CurrentPawnSpeed = CurrentPawnVelocity.Size();
	CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(CurrentPawnVelocity, Pawn->GetActorRotation());

	auto Character = Cast<APSCharacter>(Pawn);
	if (Character)
	{
		IsInAir = Character->GetMovementComponent()->IsFalling();
		CurrentPawnMotion = Character->GetCurrentCharacterMotion();
		IsRunning = Character->Running;
	}
}