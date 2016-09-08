// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "MotionControllerComponent.h"
#include "VRCameraCapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

/**********************************************************************************************************
*	Class: AVRPawn
*
*	Overview:
*		This pawn is the default pawn used for room scale VR gameplay. It contains no features exclusive
*		to any hardware setup and should not be used unless a child class of the appropriate hardware does
*		not exist.
*
*	Methods:
*		SetMotionControllerSides()
*			Assigns the motion controller that is left of the other one to LeftController. If the motion
*			controller has a sceneComponent that needs to be attached to it, it is reattached. The same
*			logic is held for RightController.
*		SetPlayerHeight()
*			Gets the players current height and stores it. This assumes the player is standing up straight.

*	Data Members:
*		UMotionControllerComponent * m_Controller1
*			One of the two motion controllers.
*		UMotionControllerComponent * m_Controller2
*			One of the two motion controllers.
*		UMotionControllerComponent * m_LeftController
*			Pointer to the left controller. This points to either m_Controller1 or m_Controller2 depending
*			on which is held in which hand of the user.
*		UMotionControllerComponent * m_RightController
*			Pointer to the right controller. This points to either m_Controller2 or m_Controller2
*			depending on which is held in which hand of the user.
*		UCameraComponent * m_Camera
*			Contains the head mounted display the user will use.
*		UStaticMesh * LeftHandMesh
*			The mesh used by the left hand.
*		UStaticMesh * RightHandMesh
*			The mesh used by the right hand.
*		UWidgetComponent * m_FOVRestrictorComponent
*			Used to restrict the users peripheral vision when performing actions that could result in
*			motion sickness.
*		UVRCapsuleComponent * m_CharacterCollider
*			Capsule that tracks to the camera, copying location along the XY axis and rotation. Attach to
*			this to attach to the "character".
*		UWidgetComponet * m_FOVRestrictorComponent
*			Contains the widget used to restrict field of vision when performing actions that could cause
*			simulation sickness.
**********************************************************************************************************/
UCLASS()
class HALVA_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	UMotionControllerComponent * m_Controller1;
	UMotionControllerComponent * m_Controller2;
	UMotionControllerComponent * m_LeftController;
	UMotionControllerComponent * m_RighController;
	UCameraComponent * m_Camera;
	UVRCameraCapsuleComponent * m_CharacterCollider;
	UWidgetComponent * m_FOVRestrictorComponent;
	
};
