// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"
#include "FlappyBirdGameMode.generated.h"

class UGamePlayWidget;
class APipeObstacle;
class IPlayerInterface;

UCLASS()
class FLAPPYBIRD_API AFlappyBirdGameMode : public AGameModeBase, public IGameModeInterface
{
	GENERATED_BODY()
	
public:

	virtual void StopPipesMovement() override;
	virtual void StartGame() override;
	virtual void ReUseObstacle(APipeObstacle* ObstacleToReUse) override;
	virtual void RestartGame() override;

	virtual IPlayerInterface* GetPlayerInterface() override
	{
		return PlayerInterface;
	}

	virtual UGamePlayWidget* GetGamePlayWidgetReference() override
	{
		return GamePlayWidget;
	}

protected:

	virtual void BeginPlay() override;

private:
	TArray<AActor*> PipeObstacles;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGamePlayWidget> GamePlayWidgetClass;

	UGamePlayWidget* GamePlayWidget;

	IPlayerInterface* PlayerInterface;

	UPROPERTY(EditAnywhere)
	float GravityScale;
};
