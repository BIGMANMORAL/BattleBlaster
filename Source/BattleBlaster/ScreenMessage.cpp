// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetTextMessage(FString Message)
{
	FText MessageText = FText::FromString(Message);
	MessageTextBlock->SetText(MessageText);
}
