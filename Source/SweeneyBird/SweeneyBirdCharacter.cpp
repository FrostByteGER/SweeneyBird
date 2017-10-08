// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SweeneyBird.h"
#include "Engine.h"
#include "SweeneyBirdCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);
//////////////////////////////////////////////////////////////////////////
// ASweeneyBirdCharacter

ASweeneyBirdCharacter::ASweeneyBirdCharacter()
{

	bCanAlwaysJump = true;

	// Use only Yaw from the controller and ignore the rest of the rotation.
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = true;
	this->bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	this->GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	this->GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	this->OnActorHit.AddDynamic(this, &ASweeneyBirdCharacter::Hit);

	this->GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	this->GetCharacterMovement()->GravityScale = 1.0f;
	this->GetCharacterMovement()->AirControl = 0.80f;
	this->GetCharacterMovement()->JumpZVelocity = 1000.f;
	this->GetCharacterMovement()->GroundFriction = 3.0f;
	this->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	this->GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	this->GetCharacterMovement()->bConstrainToPlane = true;
	this->GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	this->GetCharacterMovement()->bUseFlatBaseForFloorChecks = true; // TODO: IMPORTANT!

// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
// 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	this->GetSprite()->SetIsReplicated(true);
	this->bReplicates = true;
}

void ASweeneyBirdCharacter::Destroyed()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASweeneyBirdCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = this->GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if(this->GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		this->GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void ASweeneyBirdCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	

}


//////////////////////////////////////////////////////////////////////////
// Input

void ASweeneyBirdCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASweeneyBirdCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASweeneyBirdCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASweeneyBirdCharacter::TouchStopped);
}

bool ASweeneyBirdCharacter::CanJumpInternal_Implementation() const
{
	// Ensure the character isn't currently crouched.
	bool bCanJump = bCanAlwaysJump;

	const UCharacterMovementComponent* const characterMovement = GetCharacterMovement();
	// Ensure that the CharacterMovement state is valid
	bCanJump &= !bIsCrouched &&
		characterMovement &&
		characterMovement->IsJumpAllowed() &&
		!characterMovement->bWantsToCrouch &&
		(characterMovement->IsMovingOnGround() || characterMovement->IsFalling());

	return bCanJump;
}

void ASweeneyBirdCharacter::Hit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void ASweeneyBirdCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	this->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ASweeneyBirdCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ASweeneyBirdCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ASweeneyBirdCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	MoveRight(1.0f);
	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.

	const FVector playerDirection = this->GetActorLocation();
	const FVector direction = playerDirection + this->GetVelocity();
	const FRotator rotation = UKismetMathLibrary::FindLookAtRotation(playerDirection, direction);

	Controller->SetControlRotation(rotation);


	if (false)//Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

