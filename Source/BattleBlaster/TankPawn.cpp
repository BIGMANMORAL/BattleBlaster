// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

ATankPawn::ATankPawn()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	SetPlayerEnable(false);

}


// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerController = Cast<APlayerController>(GetController());

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	RotateTurret(HitResult.ImpactPoint);

}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &ATankPawn::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction,ETriggerEvent::Triggered, this, &ATankPawn::TurnInput);
		EnhancedInputComponent->BindAction(FireAction,ETriggerEvent::Started, this, &ATankPawn::Fire);
	}
}

void ATankPawn::MoveInput(const FInputActionValue& value)
{
	float InputValue = value.Get<float>();
	
	FVector DeltaLocation = FVector(0.0f,0.0f,0.0f);
	DeltaLocation.X = MoveSpeed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalOffset(DeltaLocation, true);
}

void ATankPawn::TurnInput(const FInputActionValue& value)
{
	float TurnInputValue = value.Get<float>();

	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);
	DeltaRotation.Yaw = TurnRate * TurnInputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void ATankPawn::HandleDestruction()
{
	Super::HandleDustruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnable(false);
	IsAlive = false;

}

void ATankPawn::SetPlayerEnable(bool Enable)
{
	if (PlayerController)
	{
		if (Enable)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);
		}
		PlayerController->bShowMouseCursor = Enable;
	}
}
