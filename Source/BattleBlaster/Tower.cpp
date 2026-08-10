// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATower::checkFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}

}

void ATower::checkFireCondition()
{
	if (Tank && IsInFireRange() && Tank->IsAlive)
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	bool result = false;

	if (Tank)
	{
		float distanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		result = (distanceToTank <= FireRange);
	}
	return result;
}

void ATower::HandleDestruction()
{
	Super::HandleDustruction();

	UE_LOG(LogTemp, Warning, TEXT("Tower HandleDestruction"));
	Destroy(); 
}
