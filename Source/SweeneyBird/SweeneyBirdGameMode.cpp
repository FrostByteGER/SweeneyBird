// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SweeneyBird.h"
#include "Engine.h"
#include "SweeneyBirdGameMode.h"
#include "Camera/CameraActor.h"
#include "SweeneyBirdCharacter.h"
#include "SweeneyObstacle.h"
#include "OscillateMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SweeneyPlayerController.h"
#include "SweeneyUtilityLibrary.h"
#include "WeakObjectPtrTemplates.h"

ASweeneyBirdGameMode::ASweeneyBirdGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our character
	DefaultPawnClass = ASweeneyBirdCharacter::StaticClass();	

	PlayerCameraClass = ACameraActor::StaticClass();

	PlayerControllerClass = ASweeneyPlayerController::StaticClass();

	ObstacleSpawnPosition = FVector(2250.0f, 0, 1017.0f);

	ObstacleSpawnOffsetMin = FVector(-1250.0f, 0.0f, -600.0f);
	ObstacleSpawnOffsetMax = FVector(-500.0f, 0.0f, 600.0f);
	ObstacleMovementChance = 0.25f;

	ObstacleMovementFrequencyMin = 3.0f;
	ObstacleMovementFrequencyMax = 7.5f;
	ObstacleMovementMagnitudeMin = 75.0f;
	ObstacleMovementMagnitudeMax = 150.0f;
}

void ASweeneyBirdGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayerRef = Cast<ASweeneyBirdCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerRef.IsValid()) UE_LOG(SBCriticalErrors, Error, TEXT("Player Reference is invalid!"));
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "MainCamera", actors);
	if (actors.Num() > 0) CameraRef = Cast<ACameraActor>(actors[0]);
	if(CameraRef.IsValid())
	{
		CameraViewSize.X = CameraRef->GetCameraComponent()->OrthoWidth;
		CameraViewSize.Y = CameraViewSize.X / CameraRef->GetCameraComponent()->AspectRatio;
		CameraViewSizeHalf = CameraViewSize / 2.0f;
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASweeneyObstacle::StaticClass(), Obstacles);
	for (const auto& element : Obstacles)
	{
		if(element->Tags.Contains(FName("LastObstacle")))
		{
			LastSpawnedObstacleRef = Cast<ASweeneyObstacle>(element);
			break;
		}
	}
	if (!LastSpawnedObstacleRef.IsValid()) UE_LOG(SBCriticalErrors, Error, TEXT("LastObstacle Reference is invalid!"));
}


void ASweeneyBirdGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(PlayerRef.IsValid())
	{
		const APlayerController* pcRef = Cast<APlayerController>(PlayerRef->GetController());
		if(!USweeneyUtilityLibrary::IsActorInViewport(*PlayerRef, *pcRef)) PlayerRef->Destroy();
		for (AActor* element : Obstacles)
		{
			ASweeneyObstacle* obstacle = Cast<ASweeneyObstacle>(element);
			if (LastSpawnedObstacleRef.IsValid() && obstacle->CanReset && !USweeneyUtilityLibrary::IsActorInViewport(*element, *pcRef))
			{
				DebugPrint("MOVING OBSTACLE!");
				FVector spawnLocation = ObstacleSpawnPosition;
				spawnLocation.X += LastSpawnedObstacleRef->GetActorLocation().X + FMath::FRandRange(ObstacleSpawnOffsetMin.X, 0);
				spawnLocation.Z += FMath::FRandRange(ObstacleSpawnOffsetMin.Z, ObstacleSpawnOffsetMax.Z);
				obstacle->CanReset = false;
				obstacle->SetActorLocation(spawnLocation);
				obstacle->BasePosition = spawnLocation;
				obstacle->SetTimeSinceLastRepositioning(obstacle->GetGameTimeSinceCreation());
				LastSpawnedObstacleRef = obstacle;

				obstacle->OscillateCompRef->Frequency = FMath::FRandRange(ObstacleMovementFrequencyMin, ObstacleMovementFrequencyMax);
				obstacle->OscillateCompRef->Magnitude = FMath::FRandRange(ObstacleMovementMagnitudeMin, ObstacleMovementMagnitudeMax);

				if (FMath::FRand() <= ObstacleMovementChance)
				{
					obstacle->OscillateCompRef->Deactivate();
				}
				else
				{
					obstacle->OscillateCompRef->Activate();
				}
			}
		}
	}
}
