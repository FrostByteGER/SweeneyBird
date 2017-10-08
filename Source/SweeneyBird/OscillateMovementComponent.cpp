// Fill out your copyright notice in the Description page of Project Settings.

#include "SweeneyBird.h"
#include "SweeneyObstacle.h"
#include "UnrealMathUtility.h"
#include "OscillateMovementComponent.h"
#include "SweeneyObstacle.h"


void UOscillateMovementComponent::BeginPlay()
{
	ParentRef = Cast<ASweeneyObstacle>(GetOwner());
}

void UOscillateMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!UpdatedComponent || ShouldSkipUpdate(DeltaTime)) return;
	auto sinValue = FMath::Sin(ParentRef->GetGameTimeSinceCreation()- ParentRef->GetTimeSinceLastRepositioning()) * Frequency;
	ParentRef->SetActorLocation(ParentRef->BasePosition + OscillateAxis * sinValue * Magnitude);
}
