#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PipeObstacle.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class IGameModeInterface;

UCLASS()
class FLAPPYBIRD_API APipeObstacle : public AActor
{
    GENERATED_BODY()

public:
    APipeObstacle();

protected:

    virtual void BeginPlay() override;
    
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TopTube;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BottomTube;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* TopCap;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BottomCap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
    float GapSize = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
    float MinRandomZLocation= 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
    float MaxRandomZLocation = 700.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
    float MoveSpeed = 500.0f;

    UFUNCTION(CallInEditor, Category = "Parameters")
    void InitializePipe();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* MovementComponent;

    void DeactivateMovementComponent();
    void ActivateMovementComponent();
    
    UPROPERTY(EditAnywhere, Category = "Parameters")
    UBoxComponent* ScoreBoxCollision;

    UFUNCTION()
    void OnScoreBoxEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(CallInEditor, Category = "Parameters")
    void ChangeGapSize();

    bool IsInView() const;

    IGameModeInterface* GameModeInterface;
};