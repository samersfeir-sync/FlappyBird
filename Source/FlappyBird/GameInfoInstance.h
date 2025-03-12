#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInfoInstanceInterface.h"
#include "GameInfoInstance.generated.h"

UCLASS()
class FLAPPYBIRD_API UGameInfoInstance : public UGameInstance, public IGameInfoInstanceInterface
{
	GENERATED_BODY()

public:

	virtual void SaveHighScore(int NewHighScore) override;
	virtual int LoadHighScore() override;

private:

	const FString SlotName = TEXT("HighScoreSlot");
};