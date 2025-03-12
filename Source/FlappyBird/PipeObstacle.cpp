
#include "PipeObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"

APipeObstacle::APipeObstacle()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent->SetMobility(EComponentMobility::Movable);

    TopTube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopTube"));
    TopTube->SetRenderCustomDepth(true);
    TopTube->SetupAttachment(RootComponent);
    TopTube->SetRelativeScale3D(FVector(1.3f, 1.3f, 6.0f));

    ChangeGapSize();

    BottomTube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomTube"));
    BottomTube->SetRenderCustomDepth(true);
    BottomTube->SetupAttachment(RootComponent);
    BottomTube->SetRelativeScale3D(FVector(1.3f, 1.3f, 6.0f));

    TopCap = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopCap"));
    TopCap->SetRenderCustomDepth(true);
    TopCap->SetupAttachment(TopTube);
    TopCap->SetUsingAbsoluteScale(true);
    TopCap->SetWorldScale3D(FVector(2.0f, 2.0f, 0.25f));

    BottomCap = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomCap"));
    BottomCap->SetRenderCustomDepth(true);
    BottomCap->SetupAttachment(BottomTube);
    BottomCap->SetUsingAbsoluteScale(true);
    BottomCap->SetWorldScale3D(FVector(2.0f, 2.0f, 0.25f));

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->ProjectileGravityScale = 0.0f;
    MovementComponent->InitialSpeed = MoveSpeed;
    MovementComponent->MaxSpeed = MoveSpeed;
    MovementComponent->Velocity = FVector(-1.0, 0.0f, 0.0f);
    MovementComponent->SetAutoActivate(false);

    ScoreBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ScoreBox"));
    ScoreBoxCollision->SetupAttachment(RootComponent);
    ScoreBoxCollision->OnComponentEndOverlap.AddDynamic(this, &APipeObstacle::OnScoreBoxEndOverlap);

    TopTube->OnComponentBeginOverlap.AddDynamic(this, &APipeObstacle::OnOverlapBegin);
    BottomTube->OnComponentBeginOverlap.AddDynamic(this, &APipeObstacle::OnOverlapBegin);
}

void APipeObstacle::BeginPlay()
{   
    Super::BeginPlay();

    if (GetWorld())
    {
        AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

        if (GameMode)
        {
            GameModeInterface = Cast<IGameModeInterface>(GameMode);
        }
    }

    SetActorTickEnabled(false);
    InitializePipe();
}

void APipeObstacle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!IsInView())
    {
        if (GameModeInterface)
        {
            GameModeInterface->ReUseObstacle(this);
            SetActorTickEnabled(false);
        }
    }
}

void APipeObstacle::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->Implements<UPlayerInterface>())
        {
            IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
            if (PlayerInterface)
            {
                PlayerInterface->Die();
                TopTube->OnComponentBeginOverlap.RemoveAll(this);
                BottomTube->OnComponentBeginOverlap.RemoveAll(this);
                ScoreBoxCollision->OnComponentEndOverlap.RemoveAll(this);
            }
        }
    }
}

void APipeObstacle::InitializePipe()
{    
    FVector PipeLocation = GetActorLocation();
    PipeLocation.Z = 0.0f;
    PipeLocation.Z += FMath::RandRange(MinRandomZLocation, MaxRandomZLocation);
    SetActorLocation(PipeLocation);
}

void APipeObstacle::DeactivateMovementComponent()
{
    if (MovementComponent)
    {
        MovementComponent->Deactivate();
        SetActorTickEnabled(false);
    }
}

void APipeObstacle::ActivateMovementComponent()
{
    if (MovementComponent)
    {
        MovementComponent->Activate();
    }
}

void APipeObstacle::OnScoreBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        if (OtherActor->Implements<UPlayerInterface>())
        {
            IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
            if (PlayerInterface)
            {
                if (PlayerInterface->IncrementScore())
                {
                    SetActorTickEnabled(true);
                }
            }
        }
    }
}

void APipeObstacle::ChangeGapSize()
{
    FVector TopTubeLocation = TopTube->GetRelativeLocation();
    TopTubeLocation.Z = GapSize;
    TopTube->SetRelativeLocation(TopTubeLocation);
}

bool APipeObstacle::IsInView() const
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return false;
    }

    APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
    if (!CameraManager)
    {
        return false;
    }

    FVector ActorLocation = GetActorLocation();

    FVector2D ScreenPosition;
    bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, ActorLocation, ScreenPosition);

    if (bIsOnScreen)
    {
        int ScreenWidth, ScreenHeight;
        PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

        if (ScreenPosition.X >= 0 && ScreenPosition.X <= ScreenWidth &&
            ScreenPosition.Y >= 0 && ScreenPosition.Y <= ScreenHeight)
        {
            return true; //visible
        }
    }

    return false; //not visible
}
