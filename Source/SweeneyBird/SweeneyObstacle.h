// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SweeneyObstacle.generated.h"

UCLASS()
class SWEENEYBIRD_API ASweeneyObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASweeneyObstacle();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperSpriteComponent* TopObstacle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperSpriteComponent* BottomObstacle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOscillateMovementComponent* OscillateCompRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanReset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BasePosition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	float GetTimeSinceLastRepositioning() const
	{
		return TimeSinceLastRepositioning;
	}

	void SetTimeSinceLastRepositioning(const float timeSinceLastRepositioning)
	{
		TimeSinceLastRepositioning = timeSinceLastRepositioning;
	}

private:
	float TimeSinceLastRepositioning;

	UFUNCTION()
	void BeginOverlap(AActor* SelfActor, AActor* OtherActor);

};
