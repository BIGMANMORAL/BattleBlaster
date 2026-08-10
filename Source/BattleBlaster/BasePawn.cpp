// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector TargetingVector = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator TurretRotation = FRotator(0.f, TargetingVector.Rotation().Yaw, 0.f);

	FRotator InterpRotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), TurretRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	TurretMesh->SetWorldRotation(InterpRotation);
}

void ABasePawn::Fire()
{
	FVector Spawnpoint = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Spawnpoint, SpawnRotation);
	if (Projectile)
	{
		Projectile->SetOwner(this);
	}
}

void ABasePawn::HandleDustruction()
{
	if (DeathParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
	if (DeathCameraShakeClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(DeathCameraShakeClass);
		}
	}
}
 