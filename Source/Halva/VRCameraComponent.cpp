// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "VRCameraComponent.h"

/**********************************************************************************************************
*	UVRCameraComponent()
*		Purpose:	Default constructor.
**********************************************************************************************************/
UVRCameraComponent::UVRCameraComponent()
{
	bLockToHmd = false;
}
/**********************************************************************************************************
*	void UVRCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
*		Purpose:	Updates the cameras position based on internal settings. Additionally, updates
*					HMDDelta to reflect the HMD's displacement since last frame.
*
*		Parameters:
*			float DeltaTime
*				The time since the last tick.
*			FMinimalViewInfo& DesiredView
*				The view trying to be achieved.
*
*		Changes:
*			The component's location and rotation could change.
*			m_HmdDelta
*				Updated with new change since last frame.
*			m_HmdLocation
*				Updated to the new position of the HMD.
*			m_HmdRotation
*				Updated to the new rotation of the HMD.
**********************************************************************************************************/
void UVRCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	// If the rotation is set to lock but not the location.
	if (bLockToHmdRotation && !bLockToHmd && GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHeadTrackingAllowed())
	{
		SetRelativeRotation(FRotator(0, 0, 0));
		const FTransform parentWorld = GetComponentToWorld();
		GEngine->HMDDevice->SetupLateUpdate(parentWorld, this);

		FQuat orientation;
		FVector position;
		if (GEngine->HMDDevice->UpdatePlayerCamera(orientation, position))
		{
			SetRelativeRotation(FRotator(orientation));
		}
	}

	if (GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHeadTrackingAllowed())
	{
		FQuat orientation;
		FVector position;

		// If  the position could be retrieved
		if (GEngine->HMDDevice->UpdatePlayerCamera(orientation, position))
		{
			// Update the HmdDelta and the position.
			HmdDelta = position - HmdLocation;
			HmdLocation = position;
			HmdRotation = orientation;
			bCouldGetHmd = true;
		}
		else
			bCouldGetHmd = false;
	}
}
/**********************************************************************************************************
*	bool GetLockToHmdRotation()
*		Purpose:	Getter.
**********************************************************************************************************/
bool UVRCameraComponent::GetLockToHmdRotation()
{
	return bLockToHmdRotation;
}
/**********************************************************************************************************
*	void SetLockToHmdRotation(bool Lock)
*		Purpose:	Setter.
**********************************************************************************************************/
void UVRCameraComponent::SetLockToHmdRotation(bool Lock)
{
	bLockToHmdRotation = Lock;
}
/**********************************************************************************************************
*	FVector GetHmdDelta()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector UVRCameraComponent::GetHmdDelta()
{
	return HmdDelta;
}
/**********************************************************************************************************
*	FVector GetHmdLocation()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector UVRCameraComponent::GetHmdLocation()
{
	return HmdLocation;
}
/**********************************************************************************************************
*	FVector GetHmdRotation()
*		Purpose:	Getter.
**********************************************************************************************************/
FQuat UVRCameraComponent::GetHmdRotation()
{
	return HmdRotation;
}
/**********************************************************************************************************
*	bool GetCouldGetHmd()
*		Purpose:	Getter.
**********************************************************************************************************/
bool UVRCameraComponent::GetCouldGetHmd()
{
	return bCouldGetHmd;
}