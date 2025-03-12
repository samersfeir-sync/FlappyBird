// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSave.h"

UGameSave::UGameSave()
{
	HighScore = 0;
}

void UGameSave::SetHighScore(int NewHighScore)
{
	HighScore = NewHighScore;
}
