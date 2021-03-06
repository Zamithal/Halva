// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "VRCameraCapsuleComponent.h"

/**********************************************************************************************************
*	APlayerVive()
*		Purpose:	Default constructor.
**********************************************************************************************************/
UVRCameraCapsuleComponent::UVRCameraCapsuleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Camera Capsule"));
}


// Called when the game starts
void UVRCameraCapsuleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


/**********************************************************************************************************
*	void UVRCameraCapsuleComponent::TickComponent(...)
*		Purpose:	Each tick m_Capsule is moved to  the location of m_Camera on the XY plane. If the
*					distance to the camera from here is less than m_MaxHeight, the capsule is extended up
*					to include the camera. If it is not, the current velocity of the camera is checked.
*					if the camera is determined jumping, the capsule component is moved up towards the
*					camera. If the camera is not considered jumping, the camera is instead moved down
*					to the capsule.
*
*		Parameters:
*			float DeltaTime
*				The time since the last tick.
*			ELevelTick TickType
*				The type of tick that occurred.
*			FActorComponentTickFunction * ThisTickFunciton
*				The function that called this.
*
*		Changes:
*			m_capsule
*				Could change in location or size depending on camera position.
*			m_camera
*				Could change in position.
*
*		NOTE: Jumping not implemented.
**********************************************************************************************************/
void UVRCameraCapsuleComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (m_Camera != nullptr)
	{
		// Location of capsule.
		FVector capsulePosition = this->GetComponentToWorld().GetLocation();
		FVector cameraPosition = m_Camera->GetComponentToWorld().GetLocation();

		// Mark the new location of the capsule.
		FVector newCapsulePosition = cameraPosition;

		float cameraHeight = cameraPosition.Z - capsulePosition.Z;

		m_Capsule->SetCapsuleHalfHeight(cameraHeight / 2);
		newCapsulePosition.Z = cameraPosition.Z - m_Capsule->GetUnscaledCapsuleHalfHeight();

		this->SetWorldLocation(newCapsulePosition);
		
		FRotator newCapsuleRotation = FRotator(0, 0, 0);
		newCapsuleRotation.Yaw = m_Camera->GetComponentToWorld().GetRotation().Rotator().Yaw;

		this->SetWorldRotation(newCapsuleRotation.Quaternion());
	}
}


/**********************************************************************************************************
*	void SetCamera(UCameraComponent * Camera)
*		Purpose:	Setter.
**********************************************************************************************************/
void UVRCameraCapsuleComponent::SetCamera(UCameraComponent * Camera)
{
	m_Camera = Camera;
}
/**********************************************************************************************************
*	UCameraComponent * GetCamera()
*		Purpose:	Getter.
**********************************************************************************************************/
UCameraComponent * UVRCameraCapsuleComponent::GetCamera()
{
	return m_Camera;
}
/**********************************************************************************************************
*	void SetMaxHeight(float MaxHeight)
*		Purpose:	Getter.
**********************************************************************************************************/
void UVRCameraCapsuleComponent::SetMaxHeight(float MaxHeight)
{
	m_MaxHeight = MaxHeight;
}
/**********************************************************************************************************
*	float GetMaxHeight()
*		Purpose:	Getter.
**********************************************************************************************************/
float UVRCameraCapsuleComponent::GetMaxHeight()
{
	return m_MaxHeight;
}
/**********************************************************************************************************
*	FVector GetBounds()
*		Purpose:	Getter.
**********************************************************************************************************/
UCapsuleComponent * UVRCameraCapsuleComponent::GetCapsule()
{
	return m_Capsule;
}
/**********************************************************************************************************
*	float GetCapsuleRadius()
*		Purpose:	Getter.
**********************************************************************************************************/
float UVRCameraCapsuleComponent::GetCapsuleRadius()
{
	return m_Capsule->GetUnscaledCapsuleRadius();
}
/**********************************************************************************************************
*	void SetCapsuleRadius(float NewRadius)
*		Purpose:	Setter.
**********************************************************************************************************/
void UVRCameraCapsuleComponent::SetCapsuleRadius(float NewRadius)
{
	m_Capsule->SetCapsuleRadius(NewRadius);
}