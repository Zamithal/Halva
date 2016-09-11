// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "VRPawn.h"

/**********************************************************************************************************
*	DungeonLayout()
*		Purpose:	Constructor. Creates default components for pawn.
**********************************************************************************************************/
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PlayerCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	m_PlayerCollider->SetVisibility(true);
	m_PlayerCollider->SetHiddenInGame(false);
	RootComponent = m_PlayerCollider;

	// Create motion controllers.
	m_Controller1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 1"));
	m_Controller2 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller 2"));
	m_Controller1->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	m_Controller2->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Create camera.
	m_Camera = CreateDefaultSubobject<UVRCameraComponent>(TEXT("Camera"));
	m_Camera->SetLockToHmdRotation(true);
	m_Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	m_Camera->AddLocalOffset(FVector(0, 0, m_PlayerCollider->GetUnscaledCapsuleHalfHeight()));
	// Cause the root of the object to move with the camera.
	m_Camera->GetPostCameraUpdateTickFunction()->BindUObject<AVRPawn>(this, &AVRPawn::MovePlayerColliderWithCamera);

	// Create vision restrictor.
	m_FOVRestrictorComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FOV Restrictor"));
	// 8.5 is the closest to the camera achievable but results in flickering.
	m_FOVRestrictorComponent->AddLocalOffset(FVector(11, 0, 0));
	m_FOVRestrictorComponent->AddLocalRotation(FRotator(0, 180, 0));
	m_FOVRestrictorComponent->SetWorldScale3D(FVector(1, .108, .06));
	m_FOVRestrictorComponent->SetVisibility(true);
	m_FOVRestrictorComponent->SetHiddenInGame(false);
	m_FOVRestrictorComponent->SetBlendMode(EWidgetBlendMode::Transparent);

	m_FOVRestrictorComponent->AttachToComponent(m_Camera, FAttachmentTransformRules::KeepRelativeTransform);

	m_LeftController = m_Controller1;
	m_RightController = m_Controller2;

	playerHeight = 50000;
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (FOVRestrictorWidget)
		m_FOVRestrictorComponent->SetWidget(FOVRestrictorWidget);
	
}

// Called every frame
void AVRPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

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
		deltaZ = m_Camera->GetHmdDelta();

	deltaZ.X = 0;
	deltaZ.Y = 0;

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
	FVector deltaZ = FVector(0, 0, 0);

	if (m_Camera != nullptr && m_Camera->GetCouldGetHmd())
		deltaZ = m_Camera->GetHmdDelta();

	deltaZ.X = 0;
	deltaZ.Y = 0;
	deltaZ.Z /= 2;

	if (m_PlayerCollider != nullptr && !deltaZ.IsZero())
	{
		// move the capsule half the distance.
		m_PlayerCollider->AddLocalOffset(deltaZ);

		// expand the capsule 1x the distance.
		float newCapsuleHalfHeight = m_PlayerCollider->GetUnscaledCapsuleHalfHeight();
		newCapsuleHalfHeight += deltaZ.Z;

		// move the camera half the distance.
		m_Camera->AddLocalOffset(deltaZ);
	}
}