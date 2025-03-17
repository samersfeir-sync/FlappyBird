// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameInfoInstanceInterface.h"
#include "PlayerInterface.h"

void UGamePlayWidget::UpdateScoreText(int Score)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(Score));
	}
}

void UGamePlayWidget::ShowRestartButton()
{
	if (RestartButton)
	{
		RestartButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGamePlayWidget::SetHighScoreText(int HighScore)
{
	if (HighScoreText)
	{
		FString FormattedString = FString::Printf(TEXT("High Score: %d"), HighScore);
		HighScoreText->SetText(FText::FromString(FormattedString));
	}
}

void UGamePlayWidget::DisableFlapButton()
{
	if (StartGameButton)
	{
		StartGameButton->SetIsEnabled(false);
	}
}

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartTextAnimation)
	{
		PlayAnimation(StartTextAnimation, 0.f, 0, EUMGSequencePlayMode::PingPong, 1.f, false);
	}

	if (GetWorld())
	{
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

		if (GameMode)
		{
			GameModeInterface = Cast<IGameModeInterface>(GameMode);
		}

		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance)
		{
			IGameInfoInstanceInterface* GameInfoInstance = Cast<IGameInfoInstanceInterface>(GameInstance);

			if (GameInfoInstance)
			{
				int HighScore = GameInfoInstance->LoadHighScore();
				SetHighScoreText(HighScore);
			}
		}
	}

	StartGameButton->OnClicked.AddDynamic(this, &UGamePlayWidget::StartGame);
	RestartButton->OnClicked.AddDynamic(this, &UGamePlayWidget::RestartGame);
}

void UGamePlayWidget::StartGame()
{
	if (GameModeInterface)
	{
		StartGameButton->OnClicked.RemoveAll(this);

		if (GameModeInterface->GetPlayerInterface())
		{
			IPlayerInterface* PlayerInterface = GameModeInterface->GetPlayerInterface();

			PlayerInterface->Flap();
			StartGameButton->OnPressed.AddDynamic(PlayerInterface, &IPlayerInterface::Flap);
		}

		GameModeInterface->StartGame();
		UpdateScoreText(0);
		StopAnimation(StartTextAnimation);
		FSlateFontInfo FontInfo = ScoreText->GetFont();
		FontInfo.Size = ScoreFontSize;
		ScoreText->SetFont(FontInfo);
	}
}

void UGamePlayWidget::RestartGame()
{
	if (RestartButton)
	{
		if (GameModeInterface)
		{
			GameModeInterface->RestartGame();
		}
	}
}
