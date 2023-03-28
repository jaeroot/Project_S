// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PSCharacter.h"


// Sets default values
APSCharacter::APSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Collision Capsule
	GetCapsuleComponent()->InitCapsuleSize(45.0f, 100.0f);

	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));

	// Set Movement
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// Set Control Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	ControlRotationBlocked = false;

	// Set Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	SpringArm->bUsePawnControlRotation = true;

	// Set Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> Mannequin_Anim(TEXT("/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint.UE4ASP_HeroTPP_AnimBlueprint_C"));
	if (Mannequin_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Mannequin_Anim.Class);
	}
}


// Called to bind functionality to input
void APSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APSCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APSCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("ControlRotation"), EInputEvent::IE_Pressed, this, &APSCharacter::BlockControlRotation);
	PlayerInputComponent->BindAction(TEXT("ControlRotation"), EInputEvent::IE_Released, this, &APSCharacter::ReleaseControlRotation);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APSCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APSCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APSCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APSCharacter::Turn);
}


// Called every frame
void APSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APSCharacter::Attack()
{

}


// Called when the game starts or when spawned
void APSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void APSCharacter::BlockControlRotation()
{
	ControlRotationBlocked = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}


void APSCharacter::ReleaseControlRotation()
{
	ControlRotationBlocked = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
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
}


void APSCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}


void APSCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}