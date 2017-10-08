// Fill out your copyright notice in the Description page of Project Settings.

#include "SweeneyBird.h"
#include "SweeneyObstacle.h"
#include "Engine.h"
#include "PaperFlipbookComponent.h"
#include "OscillateMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "SweeneyBirdCharacter.h"


// Sets default values
ASweeneyObstacle::ASweeneyObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision  = CreateDefaultSubobject<UBoxComponent>("BoxCollision");

	BoxCollision->SetCollisionProfileName(FName("OverlapAll"));
	BoxCollision->SetMobility(EComponentMobility::Movable);
	BoxCollision->bGenerateOverlapEvents = true;
	RootComponent = BoxCollision;
	TopObstacle = CreateDefaultSubobject<UPaperSpriteComponent>("TopObstacle");
	BottomObstacle = CreateDefaultSubobject<UPaperSpriteComponent>("BottomObstacle");
	TopObstacle->SetupAttachment(RootComponent);
	BottomObstacle->SetupAttachment(RootComponent);
	TopObstacle->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	TopObstacle->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	TopObstacle->SetCollisionObjectType(ECC_WorldDynamic);
	TopObstacle->Mobility = EComponentMobility::Movable;
	BottomObstacle->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	BottomObstacle->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	BottomObstacle->SetCollisionObjectType(ECC_WorldDynamic);
	BottomObstacle->Mobility = EComponentMobility::Movable;

	this->OnActorBeginOverlap.AddDynamic(this, &ASweeneyObstacle::BeginOverlap);

	OscillateCompRef = CreateDefaultSubobject<UOscillateMovementComponent>("MovementOscillate");
}

// Called when the game starts or when spawned
void ASweeneyObstacle::BeginPlay()
{
	Super::BeginPlay();
	FVector boxSize = TopObstacle->RelativeLocation + TopObstacle->Bounds.BoxExtent;
	boxSize.X = BoxCollision->GetScaledBoxExtent().X;
	boxSize.Y = BoxCollision->GetScaledBoxExtent().Y;

	BoxCollision->SetBoxExtent(boxSize);
	
	TimeSinceLastRepositioning = GetGameTimeSinceCreation();

	BasePosition = GetActorLocation();
}



// Called every frame
void ASweeneyObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASweeneyObstacle::BeginOverlap(AActor* SelfActor, AActor* OtherActor)
{
	ASweeneyBirdCharacter* player = Cast<ASweeneyBirdCharacter>(OtherActor);
	if (player)
	{
		player->Points += 1;
		UGameplayStatics::PlaySound2D(GetWorld(), OverlapSound);
		CanReset = true;
	}
}
