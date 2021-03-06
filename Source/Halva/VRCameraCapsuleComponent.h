// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "VRCameraCapsuleComponent.generated.h"

/**********************************************************************************************************
*	Class: VRCameraCapsuleComponent
*
*	Overview:
*		This component creates an expandable capsule component that can expand up to a maximum height.
*		This capsule will copy the XY position of the set camera. The capsule will change in size
*		depending on the height of the camera with the capsule becoming shorter as the camera approaches.
*		if the camera extends beyond range of the maximum height the camera or the capsule may be moved.
*
*	Mutators:
*		Camera
*			-Get
*			-Set
*		MaxHeight
*			-Get
*			-Set
*		Capsule
*			-Get
*		CapsuleRadius
*			-Get
*			-Set
*	Methods:
*		virtual void TickComponent(...) override;
*			Assuming a camera is set, Each tick the capsule component will be moved to the location of the
*			camera on the XY Plane. The capsule's height will be adjusted so that the top of the capsule
*			is at the location of the camera. Several additional checks will be performed if the height
*			cannot be set to keep the capsule and the camera in sync.
*		virtual void VRAddLocalOffset(...);
*			Wrapper function for moving this
*	Data Members:
*		UCapsuleComponet * m_Capsule
*			Expandable capsule component that tracks the camera.
*		UCameraComponent * m_Camera
*			Camera tracked. Capsule component expands and contracts to line up with this at the 'head'
*			position.
*		float m_MaxHeight
*			Maximum height of the capsule component. If additional height is required the camera or capsule
*			will be moved depending on circumstances.
**********************************************************************************************************/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALVA_API UVRCameraCapsuleComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVRCameraCapsuleComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//virtual void VRAddLocalOffset(FVector DeltaLocation, bool bSweep = false, FHitResult * OutSweepHitResult = nullptr, ETeleportType Teleport = ETeleportType::None);

	void SetCamera(UCameraComponent * Camera);
	UCameraComponent * GetCamera();
	void SetMaxHeight(float MaxHeight);
	float GetMaxHeight();
	UCapsuleComponent * GetCapsule();
	float GetCapsuleRadius();
	void SetCapsuleRadius(float NewRadius);

private:
	UCapsuleComponent * m_Capsule;
	UCameraComponent * m_Camera;
	float m_MaxHeight;
	
};
