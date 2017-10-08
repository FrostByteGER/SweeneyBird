// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SweeneyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SWEENEYBIRD_API ASweeneyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ASweeneyPlayerController();
	void Tick(float DeltaSeconds) override;
protected:
	void BeginPlay() override;
};
