// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SplashScreenWidget.generated.h"

class UCanvasPanel;
class UBorder;
class UMediaPlayer;
class UMediaSource;

UCLASS()
class FLAPPYBIRD_API USplashScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(EditDefaultsOnly)
	UMediaPlayer* MediaPlayer;

	UPROPERTY(EditDefaultsOnly)
	UMediaSource* MediaSource;

	UFUNCTION()
	void OnVideoFinished();

	void OpenFlappyBirdMap();
};
