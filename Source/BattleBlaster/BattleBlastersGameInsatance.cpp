// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlastersGameInsatance.h"
#include "Kismet/GameplayStatics.h"


void UBattleBlastersGameInsatance::ChangeLevel(int32 LevelIndex)
{
	if (LevelIndex > 0 && LevelIndex <= LastLevelIndex)
	{
		CurrentLevelIndex = LevelIndex;

		FString CurrentLevelName = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), *CurrentLevelName);
	}
}

void UBattleBlastersGameInsatance::LoadNextLevel()
{
	if (CurrentLevelIndex < LastLevelIndex)
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		RestartGame();
	}
}

void UBattleBlastersGameInsatance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

void UBattleBlastersGameInsatance::RestartGame()
{
	ChangeLevel(1);
}