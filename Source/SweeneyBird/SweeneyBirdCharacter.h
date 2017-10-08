// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperCharacter.h"
#include "SweeneyBirdCharacter.generated.h"

// This class is the default character for SweeneyBird, and it is responsible for all
// physical interaction between the player and the world.
//
//   The capsule component (inherited from ACharacter) handles collision with the world
//   The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
//   The Sprite component (inherited from APaperCharacter) handles the visuals

class UTextRenderComponent;

UCLASS(config=Game)
class ASweeneyBirdCharacter : public APaperCharacter
{
	GENERATED_BODY()

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void Hit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual bool CanJumpInternal_Implementation() const override;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanAlwaysJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACameraActor* MainCamera;

public:
	ASweeneyBirdCharacter();

	void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Points;
};
