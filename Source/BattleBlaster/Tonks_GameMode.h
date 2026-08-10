 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankPawn.h"
#include "ScreenMessage.h"
#include "Tonks_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATonks_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	UScreenMessage* ScreenMessageWidget;

	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	int32 CountDownDelay = 3;

	int32 countDownSeconds; 

	FTimerHandle CountDownTimerHandle;

	bool IsVictory = false;

	ATankPawn* Tank;
	int32 TowerCount;
	
	void ActorDied(AActor* DeadActor);
	void OnGameOverTimerTimeout();
	void OnCountDownTimerTimeout();
};
