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
	FQuat orientation;
	FVector position;

	// Replacement block for bLockToHmd
	if (bLockToHmdRotation && !bLockToHmd && GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHeadTrackingAllowed())
	{
		const FTransform parentWorld = CalcNewComponentToWorld(FTransform());

		// If  the position could be retrieved
		if (GEngine->HMDDevice->UpdatePlayerCamera(orientation, position))
		{
			// This should probably be rotation only.
			SetRelativeRotation(orientation);
		}
		else
		{
			// Break on disconnect?
			ResetRelativeTransform();
		}
	}

	// This block updates information on HMD location changes.
	if (GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHeadTrackingAllowed())
	{

		// Update the HmdDelta and the position.
		HmdDelta = position - HmdLocation;
		HmdLocation = position;
		HmdRotation = orientation;
		bCouldGetHmd = true;

		// Execute user code.
		PostCameraUpdateTickFunction.ExecuteIfBound(DeltaTime, DesiredView);
	}
	else
	{
		bCouldGetHmd = false;
		HmdDelta = FVector(0, 0, 0);
	}

	// Do standard camera things.
	Super::GetCameraView(DeltaTime, DesiredView);
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
/**********************************************************************************************************
*	PostCameraUpdateTickFunctionDelegaten GetPostCameraUpdateTickFunction()
*		Purpose:	Getter.
**********************************************************************************************************/
PostCameraUpdateTickFunctionDelegate * UVRCameraComponent::GetPostCameraUpdateTickFunction()
{
	return &PostCameraUpdateTickFunction;
}