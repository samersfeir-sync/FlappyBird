#include "Ground.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerInterface.h"

AGround::AGround()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	GroundMesh->SetRenderCustomDepth(true);
	GroundMesh->SetRelativeScale3D(FVector(3.0f));
	
	GroundMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GroundMesh->SetCollisionProfileName("BlockAll");

	GroundMesh->OnComponentHit.AddDynamic(this, &AGround::OnHitEvent);
}

void AGround::BeginPlay()
{
	Super::BeginPlay();
}

void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGround::OnHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (OtherActor->Implements<UPlayerInterface>())
		{
			IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
			PlayerInterface->Die();
			GroundMesh->OnComponentHit.RemoveAll(this);
		}
	}
}