// Fill out your copyright notice in the Description page of Project Settings.

#include "SweeneyBird.h"
#include "SweeneyUtilityLibrary.h"


bool USweeneyUtilityLibrary::IsActorInViewport(const AActor& actor, const APlayerController& pc)
{
	FVector2D screenLocation;
	const bool isInCam = pc.ProjectWorldLocationToScreen(actor.GetActorLocation(), screenLocation);
	if (isInCam) {
		int32 viewportX = 0;
		int32 viewportY = 0;
		pc.GetViewportSize(viewportX, viewportY);
		return !(screenLocation.X < 0.0f || screenLocation.Y < 0.0f || screenLocation.X > static_cast<float>(viewportX) || screenLocation.Y > static_cast<float>(viewportY));
	}else
	{
		return false;
	}
}
