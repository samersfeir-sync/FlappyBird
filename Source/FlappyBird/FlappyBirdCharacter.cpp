
#include "FlappyBirdCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeInterface.h"
#include "GamePlayWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameInfoInstanceInterface.h"

AFlappyBirdCharacter::AFlappyBirdCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->GravityScale = 0.0f, //1.5
    GetCharacterMovement()->AirControl = 0.0f;
    
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 600.0f;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritRoll = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bDoCollisionTest = false;
    SpringArm->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;
}

void AFlappyBirdCharacter::Flap()
{
    LaunchCharacter(FVector(0.0f, 0.0f, FlapStrength), true, true);

    if (FlapSound)
    {
        UGameplayStatics::PlaySound2D(this, FlapSound);
    }
}

// Called when the game starts or when spawned
void AFlappyBirdCharacter::BeginPlay()
{
    Super::BeginPlay();

    SetActorTickEnabled(false);
    SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

    if (GetWorld())
    {
        AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

        if (GameMode && GameMode->Implements<UGameModeInterface>())
        {
           GameModeInterface = Cast<IGameModeInterface>(GameMode);
        }
        
        UGameInstance* GameInstance = GetWorld()->GetGameInstance();

        if (GameInstance)
        {
            GameInfoInstanceInterface = Cast<IGameInfoInstanceInterface>(GameInstance);

            if (GameInfoInstanceInterface)
            {
                HighScore = GameInfoInstanceInterface->LoadHighScore();
            }
        }
    }
}


void AFlappyBirdCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    PlayerController = Cast<APlayerController>(NewController);

    if (PlayerController)
    {
        DisableInput(PlayerController);
    }
}

// Called every frame
void AFlappyBirdCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector TargetLocation = FVector(GetMesh()->GetComponentLocation().X, SpringArm->GetComponentLocation().Y, SpringArm->GetComponentLocation().Z);
    FVector CurrentLocation = SpringArm->GetComponentLocation();

    float InterpSpeed = 2.0f;
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
    SpringArm->SetRelativeLocation(NewLocation);
}

void AFlappyBirdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (PlayerController)
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
    {
        EnhancedInputComponent->BindAction(FlapAction, ETriggerEvent::Triggered, this, &AFlappyBirdCharacter::Flap);
    }
}

void AFlappyBirdCharacter::Die()
{
    if (!bDead)
    {
        if (GetMesh())
        {
            bDead = true;
            GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            GetMesh()->SetSimulatePhysics(true);
            //GetMesh()->AddImpulseAtLocation(FVector(ImpulseStrength, 0.f, 0.f), GetActorLocation());
            SetActorTickEnabled(true);

            if (DeathSound)
            {
                UGameplayStatics::PlaySound2D(this, DeathSound);
            }

            if (GameModeInterface)
            {
                GameModeInterface->StopPipesMovement();
                GameModeInterface->GetGamePlayWidgetReference()->ShowRestartButton();
                GameModeInterface->GetGamePlayWidgetReference()->DisableFlapButton();
            }

            if (Score > HighScore)
            {
                if (GameInfoInstanceInterface)
                {
                    GameInfoInstanceInterface->SaveHighScore(Score);
                }
            }
        }
    }
}

void AFlappyBirdCharacter::SetGravityScale(float NewScale)
{
    GetCharacterMovement()->GravityScale = NewScale;
}

bool AFlappyBirdCharacter::IncrementScore()
{
    if (!bDead)
    {
        Score++;

        if (ScoreIncrementSound)
        {
            UGameplayStatics::PlaySound2D(this, ScoreIncrementSound);
        }

        if (GameModeInterface)
        {
            GameModeInterface->GetGamePlayWidgetReference()->UpdateScoreText(Score);

            if (Score > HighScore)
            {
                GameModeInterface->GetGamePlayWidgetReference()->SetHighScoreText(Score);
            }

        }

        return true; //score was successfully incremented
    }

    return false; //no increment
}

void AFlappyBirdCharacter::ActivateInput()
{
    if (PlayerController)
    {
        EnableInput(PlayerController);
    }
}
