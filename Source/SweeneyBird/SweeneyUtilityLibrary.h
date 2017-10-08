// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SweeneyUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SWEENEYBIRD_API USweeneyUtilityLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	static bool IsActorInViewport(const AActor& actor, const APlayerController& pc);
	
	
};
