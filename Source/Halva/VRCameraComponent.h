// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Camera/CameraComponent.h"
#include "Engine.h"
#include "IHeadMountedDisplay.h"
#include "VRCameraComponent.generated.h"

/**********************************************************************************************************
*	Class: VRCameraComponent
*
*	Overview:
*		Camera class designed for VR use. contains additional information such as the change in HMD
*		position and the change in HMD rotation to allow more flexibility when working with roomscale VR.
*
*	Mutators:
*		LockToHmdRotation
*			-Get
*			-Set
*		HmdDelta
*			-Get
*		HmdLocation
*			-Get
*		HmdRotation
*			-Get
*		CouldGetHmd
*			-Get
*	Methods:
*		virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
*			Updates the position of the camera each frame depending on several internal options.
*			This will perform its usual functions with the addition of tracking HMDDelta and, if selected,
*			locking just the rotation to the HMD.
*	Data Members:
*		bool bLockToHmdRotation
*			Should the camera copy the hmd's rotation? This option is ignored if bLockToHMD is selected.
*		FVector HmdDelta
*			The change in location the HMD experienced since the last frame.
*		FVector HmdLocation
*			The current location of the HMD.
*		FQuat HmdRotation
*			The current rotation of the HMD.
*		bool bCouldGetHmd
*			Returns if the Hmd's data was successfully retrieved.
**********************************************************************************************************/
UCLASS()
class HALVA_API UVRCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UVRCameraComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Camera")
		virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	bool GetLockToHmdRotation();
	void SetLockToHmdRotation(bool Lock);
	FVector GetHmdDelta();
	FVector GetHmdLocation();
	FQuat GetHmdRotation();
	bool GetCouldGetHmd();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CameraSettings")
		bool bLockToHmdRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		FVector HmdDelta;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		FVector HmdLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		FQuat HmdRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		bool bCouldGetHmd;
};
