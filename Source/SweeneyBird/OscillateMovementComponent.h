// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "OscillateMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Movement), meta = (BlueprintSpawnableComponent))
class SWEENEYBIRD_API UOscillateMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OscillateAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude;

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ASweeneyObstacle* ParentRef;


};
