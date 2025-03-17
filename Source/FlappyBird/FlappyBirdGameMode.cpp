// Fill out your copyright notice in the Description page of Project Settings.


#include "FlappyBirdGameMode.h"
#include "FlappyBirdCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PipeObstacle.h"
#include "PlayerInterface.h"
#include "GamePlayWidget.h"

void AFlappyBirdGameMode::StopPipesMovement()
{
    for (AActor* Actor : PipeObstacles)
    {
        APipeObstacle* PipeObstacle = Cast<APipeObstacle>(Actor);
        if (PipeObstacle)
        {
            PipeObstacle->DeactivateMovementComponent();
        }
    }
}

void AFlappyBirdGameMode::StartGame()
{
    for (AActor* Actor : PipeObstacles)
    {
        APipeObstacle* PipeObstacle = Cast<APipeObstacle>(Actor);
        if (PipeObstacle)
        {
            PipeObstacle->ActivateMovementComponent();
        }
    }

    if (PlayerInterface)
    {
        PlayerInterface->SetGravityScale(GravityScale);
        PlayerInterface->ActivateInput();
    }
}

void AFlappyBirdGameMode::ReUseObstacle(APipeObstacle* ObstacleToReUse)
{
    int32 Index = PipeObstacles.IndexOfByKey(ObstacleToReUse);
    if (Index != INDEX_NONE)
    {
        AActor* LastActor = PipeObstacles.Last();
        APipeObstacle* LastPipeObstacle = Cast<APipeObstacle>(LastActor);

        if (LastPipeObstacle)
        {
            FVector LastIndexLocation = LastPipeObstacle->GetActorLocation();
            FVector NewLocation = LastIndexLocation + FVector(1000.0f, 0.0f, 0.0f);
            ObstacleToReUse->SetActorLocation(NewLocation);
            ObstacleToReUse->InitializePipe();
        }

        PipeObstacles.RemoveAt(Index);
        PipeObstacles.Add(ObstacleToReUse);
    }
}

void AFlappyBirdGameMode::RestartGame()
{
    AGameMode::RestartGame();
}

void AFlappyBirdGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GamePlayWidgetClass)
    {
        GamePlayWidget = CreateWidget<UGamePlayWidget>(GetWorld(), GamePlayWidgetClass);

        if (GamePlayWidget)
        {
            GamePlayWidget->AddToViewport();
        }
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APipeObstacle::StaticClass(), PipeObstacles);

    if (!PipeObstacles.IsEmpty())
    {
       FVector CurrentLocation = PipeObstacles[0]->GetActorLocation();
       PipeObstacles[0]->SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, 100.0f));
    }
    
    AFlappyBirdCharacter* PlayerCharacter = Cast<AFlappyBirdCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlappyBirdCharacter::StaticClass()));

    if (PlayerCharacter)
    {
        PlayerInterface = Cast<IPlayerInterface>(PlayerCharacter);

        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        if (PlayerController)
        {
            PlayerController->Possess(PlayerCharacter);

            int32 ViewportSizeX, ViewportSizeY;
            PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

            // Calculate the middle of the screen in screen coordinates
            FVector2D ScreenMiddle = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);

            // Convert the screen middle to a world position
            FVector WorldLocation;
            FVector WorldDirection;
            PlayerController->DeprojectScreenPositionToWorld(
                ScreenMiddle.X, ScreenMiddle.Y, WorldLocation, WorldDirection);

            // Get the current location of the player character
            FVector CurrentLocation = PlayerCharacter->GetActorLocation();

            // Create a new location with the same X and Y, but with the Z from the middle of the screen
            FVector NewLocation = FVector(CurrentLocation.X, CurrentLocation.Y, WorldLocation.Z);

            // Set the player character's new location
            PlayerCharacter->SetActorLocation(NewLocation);
            
        }
    }
}

