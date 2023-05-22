// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/PSVehicle.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/PSCharacter.h"
#include "Kismet/KismetMathLibrary.h"

APSVehicle::APSVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Vehicle Mesh
	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleMesh"));
	RootComponent = VehicleMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Vehicle(TEXT("/Game/Assets/Meshes/CarUpper"));
	if (SM_Vehicle.Succeeded())
	{
		VehicleMesh->SetStaticMesh(SM_Vehicle.Object);
		VehicleMesh->SetSimulatePhysics(true);
		VehicleMesh->SetMassOverrideInKg(NAME_None, 1200.0f);
		VehicleMesh->SetAngularDamping(1.0f);
	}

	// Set Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(VehicleMesh);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	SpringArm->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));
	SpringArm->bEnableCameraLag = true;

	// Set Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Set Arrow
	Arrow_FR = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_FR"));
	Arrow_FR->SetupAttachment(VehicleMesh, FName("WheelFR"));
	Arrow_FL = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_FL"));
	Arrow_FL->SetupAttachment(VehicleMesh, FName("WheelFL"));
	Arrow_RR = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_RR"));
	Arrow_RR->SetupAttachment(VehicleMesh, FName("WheelRR"));
	Arrow_RL = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_RL"));
	Arrow_RL->SetupAttachment(VehicleMesh, FName("WheelRL"));
	
	WheelArrowComponentHolder = {Arrow_FR, Arrow_FL, Arrow_RR, Arrow_RL};
	for (auto Arrow : WheelArrowComponentHolder)
	{
		Arrow->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	}
	
	// Set Wheel Scene
	WheelScene_FR = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_FR"));
	WheelScene_FR->SetupAttachment(Arrow_FR);
	WheelScene_FL = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_FL"));
	WheelScene_FL->SetupAttachment(Arrow_FL);
	WheelScene_RR = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_RR"));
	WheelScene_RR->SetupAttachment(Arrow_RR);
	WheelScene_RL = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_RL"));
	WheelScene_RL->SetupAttachment(Arrow_RL);
	
	WheelSceneComponentHolder = {WheelScene_FR, WheelScene_FL, WheelScene_RR, WheelScene_RL};
	for (auto WheelScene : WheelSceneComponentHolder)
	{
		WheelScene->SetRelativeLocation(FVector(55.0f, 0.0f, 0.0f));
	}

	// Set Wheel Mesh
	Wheel_FR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_FR"));
	Wheel_FR->SetupAttachment(WheelScene_FR);
	Wheel_FL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_FL"));
	Wheel_FL->SetupAttachment(WheelScene_FL);
	Wheel_RR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_RR"));
	Wheel_RR->SetupAttachment(WheelScene_RR);
	Wheel_RL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel_RL"));
	Wheel_RL->SetupAttachment(WheelScene_RL);
	
	WheelMeshComponentHolder = {Wheel_FR, Wheel_FL, Wheel_RR, Wheel_RL};
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Wheel(TEXT("/Game/Assets/Meshes/carTire"));
	if (Wheel.Succeeded())
	{
		for (auto WheelMesh : WheelMeshComponentHolder)
		{
			WheelMesh->SetStaticMesh(Wheel.Object);
			WheelMesh->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}


}

void APSVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

void APSVehicle::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MinLength = RestLength - SpringLength;
	MaxLength = RestLength + SpringLength;
	
	const FName TraceTag("WheelTraceTag");
	LineTraceCollisionQuery.TraceTag = TraceTag;
	LineTraceCollisionQuery.AddIgnoredActor(this);
}

void APSVehicle::UnPossessed()
{
	Super::UnPossessed();

	
	// Experimental Function
	SetActorEnableCollision(false);
	Character->SetActorEnableCollision(true);
	Destroy();
}

void APSVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int WheelIndex = 0; WheelIndex < MaxWheelNum; WheelIndex++)
	{
		UpdateVehicleForce(WheelIndex, DeltaTime);
	}

}

void APSVehicle::UpdateVehicleForce(int WheelIndex, float DeltaTime)
{
	if (!WheelMeshComponentHolder.IsValidIndex(WheelIndex) || !WheelSceneComponentHolder.IsValidIndex(WheelIndex) || !WheelMeshComponentHolder.IsValidIndex(WheelIndex))
	{
		return;
	}

	auto WheelArrow = WheelArrowComponentHolder[WheelIndex];
	
	FHitResult HitResult;
	FVector StartTrace = WheelArrow->GetComponentLocation();
	FVector EndTrace = WheelArrow->GetForwardVector() * (MaxLength + WheelRadius) + StartTrace;
	bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		ECC_Camera,
		LineTraceCollisionQuery
	);

#if ENABLE_DRAW_DEBUG
	
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(),
		StartTrace,
		EndTrace,
		DrawColor,
		false,
		5.0f
	);
	
#endif

	float CurrentSpringLength = WheelSceneComponentHolder[WheelIndex]->GetRelativeLocation().X;
	float CurrentSteeringAngle = UKismetMathLibrary::MapRangeClamped(RightAxisValue, -1, 1, MaxSteeringAngle * (-1), MaxSteeringAngle);

	// Suspension, Damper
	if (HitResult.IsValidBlockingHit())
	{
		// FVector SpringDir = -WheelArrowComponentHolder[WheelIndex]->GetUpVector();
		FVector SpringDir = GetActorUpVector();
		// FVector VehicleLinearVelocity = VehicleMesh->GetPhysicsLinearVelocityAtPoint(WheelMeshComponentHolder[WheelIndex]->GetComponentLocation());
		float LastSpringLength = CurrentSpringLength;
		CurrentSpringLength = UKismetMathLibrary::Clamp(HitResult.Distance - WheelRadius, MinLength, MaxLength);
		float Offset = RestLength - CurrentSpringLength;
		float Velocity = (CurrentSpringLength - LastSpringLength) / DeltaTime;
		float Force = (Offset * SpringForceConst) - (Velocity * DamperForceConst);

		// WheelSceneComponentHolder[WheelIndex]->AddRelativeLocation(SpringDir);
		VehicleMesh->AddForceAtLocation(SpringDir * Force, WheelSceneComponentHolder[WheelIndex]->GetComponentLocation());
	}
	else
	{
		CurrentSpringLength = MaxLength;
	}

	// MoveForward, Brake
	FVector VehicleForwardForce = GetActorForwardVector() * ForwardAxisValue * ForwardForceConst;	
	if (bBrakeApplied)
	{
		VehicleForwardForce = VehicleMesh->GetPhysicsLinearVelocity() * (-1) * BrakeConst;
	}
	VehicleMesh->AddForce(VehicleForwardForce);

	// Steering
	if (VehicleMesh->GetPhysicsLinearVelocity().Size() * 0.036 > 5)
	{
		if (ForwardAxisValue > 0)
		{
			VehicleMesh->AddTorqueInDegrees(FVector(0.0f, 0.0f, CurrentSteeringAngle), NAME_None, true);
		}
		else
		{
			VehicleMesh->AddTorqueInDegrees(FVector(0.0f, 0.0f, -CurrentSteeringAngle), NAME_None, true);
		}
	}

	// Wheel Location and Rotation
	if (WheelIndex < 2)
	{
		WheelSceneComponentHolder[WheelIndex]->SetRelativeRotation(FRotator(0.0f, 0.0f, CurrentSteeringAngle));
	}
	WheelSceneComponentHolder[WheelIndex]->SetRelativeLocation(FVector(CurrentSpringLength, 0.0f, 0.0f));
	WheelSceneComponentHolder[WheelIndex]->GetChildComponent(0)->AddLocalRotation(FRotator((-1) * 360 * VehicleMesh->GetPhysicsLinearVelocity().X * DeltaTime / (2 * 3.14 * WheelRadius), 0.0f, 0.0f));
}

void APSVehicle::MoveForward(float Value)
{
	ForwardAxisValue = Value;
}

void APSVehicle::MoveRight(float Value)
{
	RightAxisValue = Value;
}

void APSVehicle::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APSVehicle::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APSVehicle::HandbrakePressed()
{
	bBrakeApplied = true;
}

void APSVehicle::HandbrakeReleased()
{
	bBrakeApplied = false;
}

void APSVehicle::PossessCharacter()
{
	// Experimental Function
	Controller->Possess(Character);
}

void APSVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APSVehicle::HandbrakePressed);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &APSVehicle::HandbrakeReleased);
	PlayerInputComponent->BindAction(TEXT("PossessCharacter"), EInputEvent::IE_Pressed, this, &APSVehicle::PossessCharacter);
	
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APSVehicle::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APSVehicle::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUP"), this, &APSVehicle::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APSVehicle::Turn);
}

