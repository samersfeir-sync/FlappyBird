// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

class UButton;
class UTextBlock;
class IGameModeInterface;

UCLASS()
class FLAPPYBIRD_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateScoreText(int Score);
	void ShowRestartButton();
	void SetHighScoreText(int HighScore);

private:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void RestartGame();

	UPROPERTY(meta=(BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HighScoreText;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	IGameModeInterface* GameModeInterface;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* StartTextAnimation;

	UPROPERTY(EditDefaultsOnly)
	int ScoreFontSize = 200;
};
