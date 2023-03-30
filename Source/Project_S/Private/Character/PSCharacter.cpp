// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PSCharacter.h"


// Sets default values
APSCharacter::APSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Collision Capsule
	StandHalfHeight = 100.0f;
	CrouchHalfHeight = 70.0f;
	ProneHalfHeight = 45.0f;
	Radius = 45.0f;
	GetCapsuleComponent()->InitCapsuleSize(Radius, StandHalfHeight);

	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -StandHalfHeight), FRotator(0.0f, -90.0f, 0.0f));

	// Set Movement
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	//GetCharacterMovement()->CrouchedHalfHeight = 60.0f;
	CurrentCharacterMotion = ECharacterMotion::Stand;
	Running = false;
	RunInput = false;
	MovingBack = false;
	MovingSide = false;

	// Set Control Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	ControlRotationBlocked = false;
	ControlRotation = FRotator::ZeroRotator;

	// Set Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	SpringArm->bUsePawnControlRotation = true;
	//SpringArm->bEnableCameraLag = true;

	// Set Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> Mannequin_Anim(TEXT("/Game/Project_S/Animations/PSCharacterAnimBlueprint.PSCharacterAnimBlueprint_C"));
	if (Mannequin_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Mannequin_Anim.Class);
	}

	// Set Crouch and Prone interp
	CrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CROUCHTIMELINE"));
	ProneTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PRONETIMELINE"));
	CrouchTimelineFunction.BindUFunction(this, FName("CrouchInterp"));
	ProneTimelineFunction.BindUFunction(this, FName("ProneInterp"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Crouch_Curve(TEXT("/Game/Project_S/Animations/CrouchCurve.CrouchCurve"));
	if (Crouch_Curve.Succeeded())
	{
		CrouchCurve = Crouch_Curve.Object;
		CrouchTimeline->AddInterpFloat(CrouchCurve, CrouchTimelineFunction);
	}
	CrouchTimeline->SetLooping(false);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Prone_Curve(TEXT("/Game/Project_S/Animations/ProneCurve.ProneCurve"));
	if (Crouch_Curve.Succeeded())
	{
		ProneCurve = Prone_Curve.Object;
		ProneTimeline->AddInterpFloat(ProneCurve, ProneTimelineFunction);
	}
	ProneTimeline->SetLooping(false);
}


// Called to bind functionality to input
void APSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APSCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APSCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("ControlRotation"), EInputEvent::IE_Pressed, this, &APSCharacter::BlockControlRotation);
	PlayerInputComponent->BindAction(TEXT("ControlRotation"), EInputEvent::IE_Released, this, &APSCharacter::ReleaseControlRotation);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &APSCharacter::DoCrouch);
	PlayerInputComponent->BindAction(TEXT("Prone"), EInputEvent::IE_Pressed, this, &APSCharacter::DoProne);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &APSCharacter::Run);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &APSCharacter::StopRun);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APSCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APSCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APSCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APSCharacter::Turn);
}


// Called every frame
void APSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MovingBack && !MovingSide && RunInput && CurrentCharacterMotion == ECharacterMotion::Stand)
	{
		Running = true;
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	}
	else
	{
		Running = false;

		if (CurrentCharacterMotion == ECharacterMotion::Stand)
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}


void APSCharacter::Jump()
{
	if (CurrentCharacterMotion == ECharacterMotion::Stand)
	{
		Super::Jump();
	}
	else
	{
		SetCharacterMotion(ECharacterMotion::Stand);
	}
}


void APSCharacter::Attack()
{

}


ECharacterMotion APSCharacter::GetCurrentCharacterMotion()
{
	return CurrentCharacterMotion;
}


// Called when the game starts or when spawned
void APSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void APSCharacter::SetCharacterMotion(ECharacterMotion NewCharacterMotion)
{
	PSCHECK(CurrentCharacterMotion != NewCharacterMotion);

	bool Crouched = false;
	if (CurrentCharacterMotion == ECharacterMotion::Crouch)
		Crouched = true;

	CurrentCharacterMotion = NewCharacterMotion;

	switch (CurrentCharacterMotion)
	{
	case ECharacterMotion::Stand:
		Crouched ? CrouchTimeline->ReverseFromEnd() : ProneTimeline->ReverseFromEnd();		
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		break;
	case ECharacterMotion::Crouch:
		CrouchTimeline->PlayFromStart();
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		break;
	case ECharacterMotion::Prone:
		ProneTimeline->PlayFromStart();
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
		break;
	default:
		break;
	}
}


void APSCharacter::BlockControlRotation()
{
	ControlRotationBlocked = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;
	ControlRotation = GetControlRotation();
	bUseControllerRotationYaw = false;
}


void APSCharacter::ReleaseControlRotation()
{
	ControlRotationBlocked = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Controller->SetControlRotation(ControlRotation);
	bUseControllerRotationYaw = true;
}


void APSCharacter::DoCrouch()
{
	if (CurrentCharacterMotion == ECharacterMotion::Stand && !GetCharacterMovement()->IsFalling())
	{
		SetCharacterMotion(ECharacterMotion::Crouch);
	}
	else if (CurrentCharacterMotion == ECharacterMotion::Crouch)
	{
		if (!CanStand(CrouchHalfHeight))
		{
			SetCharacterMotion(ECharacterMotion::Stand);
		}
		else
		{
			PSLOG(Warning, TEXT("Cannot Crouch"));
		}
	}
	else
	{
		PSLOG(Warning, TEXT("Cannot Crouch"));
	}
}


void APSCharacter::DoProne()
{
	if (CurrentCharacterMotion == ECharacterMotion::Stand && !GetCharacterMovement()->IsFalling())
	{
		SetCharacterMotion(ECharacterMotion::Prone);
	}
	else if (CurrentCharacterMotion == ECharacterMotion::Prone)
	{
		if (!CanStand(ProneHalfHeight))
		{
			SetCharacterMotion(ECharacterMotion::Stand);
		}
		else
		{
			PSLOG(Warning, TEXT("Cannot Prone"));
		}
	}
	else
	{
		PSLOG(Warning, TEXT("Cannot Prone"));
	}
}


void APSCharacter::Run()
{
	RunInput = true;
}


void APSCharacter::StopRun()
{
	RunInput = false;
}


void APSCharacter::UpDown(float NewAxisValue)
{
	if (ControlRotationBlocked)
	{
		AddMovementInput(FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X), NewAxisValue);
	}
	else
	{
		AddMovementInput(FRotationMatrix(FRotator(0, GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X), NewAxisValue);
	}

	MovingBack = NewAxisValue < 0.0f;
}


void APSCharacter::LeftRight(float NewAxisValue)
{
	if (ControlRotationBlocked)
	{
		AddMovementInput(FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::Y), NewAxisValue);
	}
	else
	{
		AddMovementInput(FRotationMatrix(FRotator(0, GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y), NewAxisValue);
	}

	MovingSide = NewAxisValue != 0.0f;
}


void APSCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}


void APSCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}


void APSCharacter::CrouchInterp(float Value)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(StandHalfHeight, CrouchHalfHeight, Value));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, FMath::Lerp(-StandHalfHeight, -CrouchHalfHeight, Value)));
}


void APSCharacter::ProneInterp(float Value)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(StandHalfHeight, ProneHalfHeight, Value));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, FMath::Lerp(-StandHalfHeight, -ProneHalfHeight, Value)));
}


bool APSCharacter::CanStand(float HalfHeight)
{
	float Height = StandHalfHeight * 2 - HalfHeight - Radius;
	FHitResult HitResult;
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = (GetActorUpVector() * Height) + StartTrace;
	/*bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		ECollisionChannel::ECC_Visibility
	);*/

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(Radius));


# if ENABLE_DRAW_DEBUG

	FVector Center = (StartTrace + EndTrace) / 2;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(GetActorUpVector()).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	/*DrawDebugLine(GetWorld(),
		StartTrace,
		EndTrace,
		DrawColor,
		false,
		5.0f
	);*/

	DrawDebugCapsule(GetWorld(),
		Center,
		Height * 0.5 + Radius,
		Radius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

# endif

	return bResult;
}