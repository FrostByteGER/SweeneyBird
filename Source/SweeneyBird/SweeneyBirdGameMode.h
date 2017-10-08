// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/GameModeBase.h"
#include "SweeneyObstacle.h"
#include "SweeneyBirdCharacter.h"
#include "SweeneyBirdGameMode.generated.h"

// The GameMode defines the game being played. It governs the game rules, scoring, what actors
// are allowed to exist in this game type, and who may enter the game.
//
// This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of SweeneyBirdCharacter

UCLASS(minimalapi)
class ASweeneyBirdGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASweeneyBirdGameMode();

	/** The PlayerController class used when spectating a network replay. */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<ACameraActor> PlayerCameraClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	TWeakObjectPtr<ASweeneyObstacle> LastSpawnedObstacleRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	TArray<AActor*> Obstacles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	TWeakObjectPtr<ASweeneyBirdCharacter> PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	TWeakObjectPtr<ACameraActor> CameraRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	FVector2D CameraViewSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	FVector2D CameraViewSizeHalf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	FVector ObstacleSpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	FVector ObstacleSpawnOffsetMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	FVector ObstacleSpawnOffsetMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float ObstacleMovementChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float ObstacleMovementFrequencyMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float ObstacleMovementFrequencyMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float ObstacleMovementMagnitudeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	float ObstacleMovementMagnitudeMax;

private:
	virtual void Tick(float DeltaSeconds) override;


protected:
	virtual void BeginPlay() override;
};
