// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class APipeObstacle;
class UGamePlayWidget;
class IPlayerInterface;

class FLAPPYBIRD_API IGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void StopPipesMovement() = 0;

	virtual void StartGame() = 0;

	virtual void ReUseObstacle(APipeObstacle* ObstacleToReUse) = 0;

	virtual UGamePlayWidget* GetGamePlayWidgetReference() = 0;

	virtual void RestartGame() = 0;

	virtual IPlayerInterface* GetPlayerInterface() = 0;
};
