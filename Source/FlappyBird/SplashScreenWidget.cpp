// Fill out your copyright notice in the Description page of Project Settings.


#include "SplashScreenWidget.h"
#include "MediaPlayer.h"
#include "Kismet/GameplayStatics.h"


void USplashScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MediaPlayer && MediaSource)
    {
        MediaPlayer->OpenSource(MediaSource);

        MediaPlayer->OnEndReached.AddDynamic(this, &USplashScreenWidget::OnVideoFinished);
    }
}

void USplashScreenWidget::OnVideoFinished()
{

    FTimerHandle TimerHandle_OpenLevel;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_OpenLevel, this, &USplashScreenWidget::OpenFlappyBirdMap, 0.5f, false);
}


void USplashScreenWidget::OpenFlappyBirdMap()
{
    UGameplayStatics::OpenLevel(this, FName("FlappyBirdMap"));
}
