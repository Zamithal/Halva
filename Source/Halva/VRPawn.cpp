// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "VRPawn.h"

/**********************************************************************************************************
*	AVRPawn()
*		Purpose:	Constructor. Creates default components for pawn.
**********************************************************************************************************/
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitBody();
	InitVision();
	InitHands();

	MaximumHealth = 100;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidgetComponent->AttachToComponent(m_StaticForwardAttachmentPoint, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBarWidgetComponent->SetRelativeRotation(FRotator(90, 0, 90));
}
/**********************************************************************************************************
*	OnConstruction(const FTransform& Transform)
*		Purpose:	UE4 Construction Script
**********************************************************************************************************/
void AVRPawn::OnConstruction(const FTransform& Transform)
{
	playerHeight = 50000;
}
// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVRPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(class UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

}

/**********************************************************************************************************
*	InitHands()
*		Purpose:	Constructor sub-function. Initializes pawn parts.
**********************************************************************************************************/
void AVRPawn::InitHands()
{

	// Create motion controllers.
	m_Controller1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 1"));
	m_Controller2 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 2"));
	m_Controller1->AttachToComponent(m_RoomAttachmentPoint, FAttachmentTransformRules::KeepRelativeTransform);
	m_Controller2->AttachToComponent(m_RoomAttachmentPoint, FAttachmentTransformRules::KeepRelativeTransform);

	// Assign motion controllers
	m_LeftController = m_Controller1;
	m_RightController = m_Controller2;

	// Assign hands
	m_LeftController->Hand = EControllerHand::Left;
	m_RightController->Hand = EControllerHand::Right;

	// Turn off overlaps.
	m_LeftController->bGenerateOverlapEvents = false;
	m_RightController->bGenerateOverlapEvents = false;

	// Create Grab colliders.
	m_LeftHandGrabCollider = CreateDefaultSubobject<USphereComponent>(TEXT("LH Grab Collider"));
	m_RightHandGrabCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RH Grab Collider"));
	m_LeftHandGrabCollider->AttachToComponent(m_LeftController, FAttachmentTransformRules::KeepRelativeTransform);
	m_RightHandGrabCollider->AttachToComponent(m_RightController, FAttachmentTransformRules::KeepRelativeTransform);

	m_LeftHandGrabCollider->SetSphereRadius(7);
	m_RightHandGrabCollider->SetSphereRadius(7);

}
/**********************************************************************************************************
*	InitBody()
*		Purpose:	Constructor sub-function. Initializes pawn parts.
**********************************************************************************************************/
void AVRPawn::InitBody()
{
	// Create pawn parts.

	m_PlayerCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));

	//m_PlayerCollider->SetCapsuleHalfHeight(m_PlayerCollider->GetUnscaledCapsuleRadius());

	//Physics
	m_PlayerCollider->SetEnableGravity(true);
	m_PlayerCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_PlayerCollider->SetCollisionProfileName(FName("Pawn"));
	m_PlayerCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	m_PlayerCollider->BodyInstance.bLockXRotation = true;
	m_PlayerCollider->BodyInstance.bLockYRotation = true;
	m_PlayerCollider->BodyInstance.bLockZRotation = true;
	m_PlayerCollider->SetSimulatePhysics(true);
	RootComponent = m_PlayerCollider;

	// Create static forward arrow attachment point.
	m_StaticForwardAttachmentPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerForwardArrow_Static"));
	m_StaticForwardAttachmentPoint->AttachToComponent(m_PlayerCollider, FAttachmentTransformRules::KeepRelativeTransform);
	m_StaticForwardAttachmentPoint->RelativeLocation.Z = -m_PlayerCollider->GetScaledCapsuleHalfHeight();
	m_StaticForwardAttachmentPoint->bGenerateOverlapEvents = false;

	// Create a height dependent forward arrow attachment point.
	m_HeightDependantForwardAttachmentPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerForwardArrow_HeightDependent"));
	m_HeightDependantForwardAttachmentPoint->AttachToComponent(m_PlayerCollider, FAttachmentTransformRules::KeepRelativeTransform);
	m_HeightDependantForwardAttachmentPoint->bGenerateOverlapEvents = false;


	// Create room attachment point.
	m_RoomAttachmentPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RoomAttachmentPoint"));
	m_RoomAttachmentPoint->AttachToComponent(m_PlayerCollider, FAttachmentTransformRules::KeepRelativeTransform);

		// Create shoulder boxes.
	m_LeftShoulderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftShoulderBox"));
	m_RightShoulderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightShoulderBox"));
	m_LeftShoulderBox->AttachToComponent(m_HeightDependantForwardAttachmentPoint, FAttachmentTransformRules::KeepRelativeTransform);
	m_RightShoulderBox->AttachToComponent(m_HeightDependantForwardAttachmentPoint, FAttachmentTransformRules::KeepRelativeTransform);

	//Position and scale each box over the shoulder, Z is handled later by the controller as it is height dependent.
	m_LeftShoulderBox->AddLocalOffset(FVector(-30, -7, 0));
	m_RightShoulderBox->AddLocalOffset(FVector(-30, 7, 0));
	m_LeftShoulderBox->SetBoxExtent(FVector(5, 5, 15), false);
	m_RightShoulderBox->SetBoxExtent(FVector(5, 5, 15), false);
}
/**********************************************************************************************************
*	InitVision()
*		Purpose:	Constructor sub-function. Initializes pawn parts.
**********************************************************************************************************/
void AVRPawn::InitVision()
{
	// Create camera.
	m_Camera = CreateDefaultSubobject<UVRCameraComponent>(TEXT("Camera"));
	m_Camera->AttachToComponent(m_PlayerCollider, FAttachmentTransformRules::KeepRelativeTransform);

	// Create vision restrictor.
	FOVRestrictorComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FOV Restrictor"));
	FOVRestrictorComponent->AttachToComponent(m_Camera, FAttachmentTransformRules::KeepRelativeTransform);

	// 8.5 is the closest to the camera achievable but results in flickering;
	FOVRestrictorComponent->AddLocalOffset(FVector(10, 0, 0));
	FOVRestrictorComponent->AddLocalRotation(FRotator(0, 180, 0));
	FOVRestrictorComponent->SetWorldScale3D(FVector(.005, .005, .005));
	FOVRestrictorComponent->SetVisibility(true);
	FOVRestrictorComponent->SetHiddenInGame(false);
	FOVRestrictorComponent->SetBlendMode(EWidgetBlendMode::Transparent);
	FOVRestrictorComponent->bGenerateOverlapEvents = false;

	// Move the camera to the top of the collider.
	m_Camera->AddLocalOffset(FVector(0, 0, m_PlayerCollider->GetUnscaledCapsuleHalfHeight()));
	// Cause the root of the object to move with the camera.
	m_Camera->GetPostCameraUpdateTickFunction()->BindUObject<AVRPawn>(this, &AVRPawn::UpdateRoomscaleComponents);
	m_Camera->SetLockToHmdRotation(true);
}
/**********************************************************************************************************
*    float TakeDamage(float DamageAmount, FDamageEvent & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
*        Purpose:    Informs the AIController that this pawn took damage.
*
*        Parameters:
*            float DamageAmount
*				 The original, unmodified amount of damage to take.
*			 FDamageEvent&
*				 The precision of the damage type, EG head strike vs strike.
*			 AController* EventInstigator
*				 The controller belonging to the actor who triggered the damage.
*			 AActor* DamagingCauser
*				 The actor that actually caused the damage.
*
*        Return:
*				Returns the amount of damage dealt.
**********************************************************************************************************/
float AVRPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Controller != nullptr)
		return Controller->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	else
		return 0;
}
/**********************************************************************************************************
*    void ModifyHealth_Implementation(float Amount)
*        Purpose:    Informs the AIController that this pawn should modify it's health. This typically is
*					 not direct damage.
*
*        Parameters:
*            float Amount
*				The amount to modify health by.
**********************************************************************************************************/
void AVRPawn::ModifyHealth_Implementation(float Amount)
{
	IPotionInterface* controller = Cast<IPotionInterface>(Controller);

	if (controller != nullptr)
	{
		controller->Execute_ModifyHealth(Controller, Amount);
	}

}
/**********************************************************************************************************
*    void UpdateRoomscaleComponents()
*        Purpose:    Modifies several components so that they maintain a logical position when the player
*					 moves in roomscale VR. This adjusts the player's collider box, the rooms attachment
*					 point, and the player attachment point.
*
*        Parameters:
*			float DeltaTime
*				Time since the last frame.
*			FMinimalViewInfo& DesiredView
*				View the camera is trying to achieve. Required to match delegate but ignored for this
*				function.
*
*        Changes:
*            m_PlayerCollider
*				Location will be updated to reflect player location changes.
*			 m_StaticForwardAttachmentPoint
*				Rotation will be updated so that it is looking "forward" for the camera.
*			 m_RoomAttachmentPoint
*				Location will be updated so that it stays in place.
**********************************************************************************************************/
void AVRPawn::UpdateRoomscaleComponents(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	MovePlayerColliderWithCamera(DeltaTime, DesiredView);
	UpdateRoomAttachmentPoint();
	UpdatePlayerAttachmentPoints();
}
/**********************************************************************************************************
*	void MovePlayerColliderWithCamera(float DeltaTime, FMinimalViewInfo& DesiredView)
*		Purpose:	This function is called each time the HMD's position is updated. This function moves
*					moves the player collider with the HMD to allow the player to use room scale VR and
*					still experience interactions such as physics and collision.
*		Parameters:
*			float DeltaTime
*				Time since the last frame.
*			FMinimalViewInfo& DesiredView
*				View the camera is trying to achieve. Required to match delegate but ignored for this
*				function.
*
*		Changes:
*			m_PlayerCollider
*				The location of this component will change. The height may change as well.
**********************************************************************************************************/
void AVRPawn::MovePlayerColliderWithCamera(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	CopyCameraXYToPlayerCollider();
	ManageCrouchAndJump();
}
/**********************************************************************************************************
*	void CopyCameraXYToPlayerCollider()
*		Purpose:	Moves the player collider along the XY axis to keep it in-line with the HMD. If the
*					HMD could not be tracked this frame or did not move, the position of the player
*					collider is not updated.
*
*		Changes:
*			m_PlayerCollider
*				The location of this component will change.
**********************************************************************************************************/
void AVRPawn::CopyCameraXYToPlayerCollider()
{
	FVector deltaXY = FVector(0, 0, 0);

	if (m_Camera != nullptr && m_Camera->GetCouldGetHmd())
		deltaXY = m_Camera->GetHmdDelta();

	deltaXY.Z = 0;

	if (m_PlayerCollider != nullptr && !deltaXY.IsZero())
		m_PlayerCollider->AddLocalOffset(deltaXY);
}
/**********************************************************************************************************
*	void ManageCrouchAndJump()
*		Purpose:	Determines if the player is currently jumping or crouching and modifies the collision
*					capsule accordingly.
*
*		Changes:
*			m_PlayerCollider
*				The location of this component will change along the Z axis. The height could change.
**********************************************************************************************************/
void AVRPawn::ManageCrouchAndJump()
{
	if (m_Camera != nullptr && m_Camera->GetCouldGetHmd())
	{
		FVector hmdDelta = m_Camera->GetHmdDelta();
		FVector hmdLocation = m_Camera->GetHmdLocation();

		// If the Hmd is airborne but the character is not.
		if (hmdLocation.Z > playerHeight && !IsFalling())
		{
			// Move the collider to the camera.
			AdjustPlayerColliderLocation();
		}
		else
			// Change the height of the collider to match the camera.
			AdjustPlayerColliderHeight();

		AdjustCameraLocation();
	}
}
/**********************************************************************************************************
*	bool IsFalling()
*		Purpose:	Returns if the player is falling or not.
*
*		Return:
*			Returns true if the player's Z velocity is negative.
**********************************************************************************************************/
bool AVRPawn::IsFalling()
{
	if (m_PlayerCollider != nullptr && m_PlayerCollider->GetComponentVelocity().Z < 0)
		return true;
	else
		return false;
}
/**********************************************************************************************************
*	void AdjustPlayerColliderLocation()
*		Purpose:	Changes the location of the player collider along the Z axis. The location is set so
*					that the top of the capsule is at the position of the HMD.
*		Changes:
*			m_PlayerCollider
*				The location of this component will change along the Z axis. This change is proportional
*				to any changes the HMD experienced this frame.
**********************************************************************************************************/
void AVRPawn::AdjustPlayerColliderLocation()
{
	FVector deltaZ = FVector(0, 0, 0);

	if (m_Camera != nullptr && m_Camera->GetCouldGetHmd())
		deltaZ.Z = m_Camera->GetHmdDelta().Z;

	if (m_PlayerCollider != nullptr && !deltaZ.IsZero())
		m_PlayerCollider->AddLocalOffset(deltaZ);

}
/**********************************************************************************************************
*	void AdjustPlayerColliderHeight()
*		Purpose:	Adjusts the height of the player collider so that the top is aligned with the HMD.
*					m_Camera is adjusted to accommodate this new capsule height as well.
*		Changes:
*			m_PlayerCollider
*				The location and height of this component is adjusted so that its bottom remains in the
*				same location but its top reflects the change experienced by the HMD
*			m_Camera
*				Location adjusted so that it remains at the top of the player collider.
**********************************************************************************************************/
void AVRPawn::AdjustPlayerColliderHeight()
{
	FVector halfDeltaZ = FVector(0, 0, 0);

	if (m_Camera != nullptr && m_Camera->GetCouldGetHmd())
		halfDeltaZ.Z = m_Camera->GetHmdDelta().Z;

	halfDeltaZ.Z /= 2;

	if (m_PlayerCollider != nullptr && !halfDeltaZ.IsZero())
	{
		// move the capsule half the distance.
		m_PlayerCollider->AddLocalOffset(halfDeltaZ);

		// expand the capsule 1x the distance.
		float newCapsuleHalfHeight = m_PlayerCollider->GetUnscaledCapsuleHalfHeight();
		newCapsuleHalfHeight += halfDeltaZ.Z;

		m_PlayerCollider->SetCapsuleHalfHeight(newCapsuleHalfHeight);
	}
}
/**********************************************************************************************************
*    void AdjustCameraLocation()
*        Purpose:    Moves the player's camera to the top of the player collider. The camera will not be
*					 moved on the XY plane.
*
*        Changes:
*            m_Camera
*				The z offset of the camera will be moved up to the top of the player collider.
**********************************************************************************************************/
void AVRPawn::AdjustCameraLocation()
{
	m_Camera->SetRelativeLocation(FVector(0, 0, m_PlayerCollider->GetUnscaledCapsuleHalfHeight()));
}
/**********************************************************************************************************
*    void UpdatePlayerAttachmentPoints()
*        Purpose:    Updates the arrows indicating which way is forward to the player. Both the static one
*					 and the height dependent arrow are updated to reflect these new changes.
*        Changes:
*            m_StaticForwardAttachmentPoint
*				Rotation is updated to reflect the new direction the camera is facing.
**********************************************************************************************************/
void AVRPawn::UpdatePlayerAttachmentPoints()
{
	FRotator cameraDirection = m_Camera->GetRelativeTransform().Rotator();

	cameraDirection.Roll = 0;
	cameraDirection.Pitch = 0;

	m_HeightDependantForwardAttachmentPoint->SetWorldRotation(cameraDirection);
	m_StaticForwardAttachmentPoint->SetWorldRotation(cameraDirection);

	FVector capsuleChangeVector = m_Camera->GetHmdDelta();
	// location only changed by half. size was adjusted.
	capsuleChangeVector.Z /= 2;
	capsuleChangeVector.X = 0;
	capsuleChangeVector.Y = 0;

	m_StaticForwardAttachmentPoint->AddRelativeLocation(-capsuleChangeVector);
	m_HeightDependantForwardAttachmentPoint->AddRelativeLocation(capsuleChangeVector);

}
/**********************************************************************************************************
*    void UpdateRoomAttachmentPoint()
*        Purpose:    Moves the room attachment point the inverse of what the camera moved this frame.
*					 This ensures that the room attachment point stays in place.
*
*        Changes:
*            m_RoomAttachmentPoint
*				Location will be changed so that it remains in the same location. as it was before the
*				frame.
**********************************************************************************************************/
void AVRPawn::UpdateRoomAttachmentPoint()
{
    FVector capsuleChangeVector = m_Camera->GetHmdDelta();
    // location only changed by half. size was adjusted.
    capsuleChangeVector.Z /= 2;

	m_RoomAttachmentPoint->AddRelativeLocation(-capsuleChangeVector);
}
/**********************************************************************************************************
*	UMotionControllerComponent * GetLeftMotionController()
*		Purpose:	Getter.
**********************************************************************************************************/
UMotionControllerComponent *  AVRPawn::GetLeftMotionController()
{
	return m_LeftController;
}
/**********************************************************************************************************
*	UMotionControllerComponent * GetRightMotionController()
*		Purpose:	Getter.
**********************************************************************************************************/
UMotionControllerComponent * AVRPawn::GetRightMotionController()
{
	return m_RightController;
}
/**********************************************************************************************************
*	UCapsuleComponent * GetPlayerCollider()
*		Purpose:	Getter.
**********************************************************************************************************/
UCapsuleComponent * AVRPawn::GetPlayerCollider()
{
	return m_PlayerCollider;
}
/**********************************************************************************************************
*    UVRCameraComponent AVRPawn::GetPlayerCamera()
*        Purpose:    Getter.
**********************************************************************************************************/
UVRCameraComponent * AVRPawn::GetPlayerCamera()
{
	return m_Camera;
}
/**********************************************************************************************************
*    UArrowComponent * AVRPawn::GetPlayerStaticForwardArrow()
*        Purpose:    Getter.
**********************************************************************************************************/
UArrowComponent * AVRPawn::GetPlayerStaticForwardArrow()
{
	return m_StaticForwardAttachmentPoint;
}
/**********************************************************************************************************
*    UArrowComponent * AVRPawn::GetPlayerHeightDependentFowardArrow()
*        Purpose:    Getter.
**********************************************************************************************************/
UArrowComponent * AVRPawn::GetPlayerHeightDependentFowardArrow()
{
	return m_HeightDependantForwardAttachmentPoint;
}
/**********************************************************************************************************
*    USphereComponent* AVRPawn::GetLeftHandGrabCollider()
*        Purpose:    Getter.
**********************************************************************************************************/
USphereComponent* AVRPawn::GetLeftHandGrabCollider()
{
	return m_LeftHandGrabCollider;
}
/**********************************************************************************************************
*    USphereComponent* AVRPawn::GetRightHandGrabCollider()
*        Purpose:    Getter.
**********************************************************************************************************/
USphereComponent* AVRPawn::GetRightHandGrabCollider()
{
	return m_RightHandGrabCollider;
}
/**********************************************************************************************************
*    UBoxComponent* AVRPawn::GetLeftShoulderBox()
*        Purpose:    Getter.
**********************************************************************************************************/
UBoxComponent* AVRPawn::GetLeftShoulderBox()
{
	return m_LeftShoulderBox;
}
/**********************************************************************************************************
*    UBoxComponent* AVRPawn::GetRightShoulderBox()
*        Purpose:    Getter.
**********************************************************************************************************/
UBoxComponent* AVRPawn::GetRightShoulderBox()
{
	return m_RightShoulderBox;
}
/**********************************************************************************************************
*    AActor* AVRPawn::GetLeftSheathedItem()
*        Purpose:    Gets the item sheathed on the left shoulder.
**********************************************************************************************************/
AActor* AVRPawn::GetLeftSheathedItem()
{
	TArray<USceneComponent *> attachedComponents = m_LeftShoulderBox->GetAttachChildren();

	if (attachedComponents.Num() < 1)
		return nullptr;
	else
		return attachedComponents[0]->GetOwner();
}
/**********************************************************************************************************
*    AActor* AVRPawn::GetRightSheathedItem()
*        Purpose:    Gets the item sheathed on the right shoulder.
**********************************************************************************************************/
AActor* AVRPawn::GetRightSheathedItem()
{
	TArray<USceneComponent *> attachedComponents = m_RightShoulderBox->GetAttachChildren();

	if (attachedComponents.Num() < 1)
		return nullptr;
	else
		return attachedComponents[0]->GetOwner();
}