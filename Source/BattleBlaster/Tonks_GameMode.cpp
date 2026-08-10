// Fill out your copyright notice in the Description page of Project Settings.


#include "Tonks_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlastersGameInsatance.h"

void ATonks_GameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundTowers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATower::StaticClass(), FoundTowers);
	TowerCount = FoundTowers.Num();
	UE_LOG(LogTemp, Warning, TEXT("Tower Count: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn) 
	{
		Tank = Cast<ATankPawn>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode: Failed to find Player Tank!"));
		}
	}

	int32 loopIndex = 0;
	while (loopIndex < TowerCount) 
	{
		AActor* TowerActor = FoundTowers[loopIndex];
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank) 
			{
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Warning, TEXT("%s Setting the tank variable"), *Tower->GetActorNameOrLabel()); 
			}
		}
		loopIndex++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetTextMessage("Get Ready!");
		}
	}

	countDownSeconds = CountDownDelay;
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ATonks_GameMode::OnCountDownTimerTimeout, 1.0f, true);
}

void ATonks_GameMode::OnCountDownTimerTimeout()
{
	countDownSeconds--;
	if(countDownSeconds > 0)
	{
		ScreenMessageWidget->SetTextMessage(FString::FromInt(countDownSeconds));
	}
	else if (countDownSeconds == 0)
	{
		ScreenMessageWidget->SetTextMessage("Go Go Go");
		Tank->SetPlayerEnable(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ATonks_GameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		IsGameOver = true;
		
	}
	else
	{
		//Tower died
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();

			TowerCount--;
			if (TowerCount == 0)
			{
				IsVictory = true;
				IsGameOver = true;
			}
		}
	}
	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory!" : "Defeated :(";
		ScreenMessageWidget->SetTextMessage(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameModeTimeHnadler;
		GetWorldTimerManager().SetTimer(GameModeTimeHnadler, this, &ATonks_GameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

void ATonks_GameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlastersGameInsatance* BBGameInstance = Cast<UBattleBlastersGameInsatance>(GameInstance);
		if (BBGameInstance) 
		{
			if (IsVictory)
			{
				BBGameInstance->LoadNextLevel();
			}
			else
			{
				BBGameInstance->RestartCurrentLevel();
			}

		}
	}
	
}


