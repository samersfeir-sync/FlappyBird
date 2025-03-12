// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfoInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameSave.h"

void UGameInfoInstance::SaveHighScore(int NewHighScore)
{
    UGameSave* SaveGameInstance;

    SaveGameInstance = Cast<UGameSave>(UGameplayStatics::CreateSaveGameObject(UGameSave::StaticClass()));

    if (SaveGameInstance)
    {
        SaveGameInstance->SetHighScore(NewHighScore);
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
    }
}

int UGameInfoInstance::LoadHighScore()
{
    UGameSave* SaveGameInstance;

    if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        return 0;
    }

    SaveGameInstance = Cast<UGameSave>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

    if (SaveGameInstance)
    {
        return SaveGameInstance->HighScore;
    }

    return 0;
}