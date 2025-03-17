#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerInterface.h"
#include "FlappyBirdCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class IGameModeInterface;
class  IGameInfoInstanceInterface;

UCLASS()
class FLAPPYBIRD_API AFlappyBirdCharacter : public ACharacter, public IPlayerInterface
{
    GENERATED_BODY()

public:

    AFlappyBirdCharacter();

    virtual void Die() override;

    virtual void SetGravityScale(float NewScale) override;
    
    virtual bool IncrementScore() override;

    virtual void ActivateInput() override;

protected:

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, Category = "Inputs")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Inputs")
    UInputAction* FlapAction;

    UFUNCTION(BlueprintCallable)
    virtual void Flap() override;

    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    float FlapStrength = 100.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Parameters")
    float ImpulseStrength = -500.0f;

    APlayerController* PlayerController;

    virtual void PossessedBy(AController* NewController) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    int Score = 0;
    int HighScore = 0;

    IGameModeInterface* GameModeInterface;
    IGameInfoInstanceInterface* GameInfoInstanceInterface;

    bool bDead = false;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
    USoundBase* ScoreIncrementSound;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
    USoundBase* DeathSound;

    UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
    USoundBase* FlapSound;
};