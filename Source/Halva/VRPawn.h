// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "MotionControllerComponent.h"
#include "VRCameraCapsuleComponent.h"
#include "VRCameraComponent.h"
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
*	Mutators:
*		LeftMotionController
*			-Get
*		RightMotionController
*			-Get
*		PlayerCollider
*			-Get
*
*	Methods:
*		SetMotionControllerSides()
*			Assigns the motion controller that is left of the other one to LeftController. If the motion
*			controller has a sceneComponent that needs to be attached to it, it is reattached. The same
*			logic is held for RightController.
*		SetPlayerHeight()
*			Gets the players current height and stores it. This assumes the player is standing up straight.
*		void MovePlayerColliderWithCamera(float DeltaTime, FMinimalViewInfo& DesiredView)
*			Moves the player collider so that the player can do things in room scale and have their actions
*			reflected in game.
*		void CopyCameraXYToPlayerCollider()
*			Copies the player's roomscale movements to the player collider along the XY axis.
*		void ManageCrouchAndJump()
*			Determines if the player is crouching or jumping and calls the appropriate function.
*		bool IsFalling();
*			Returns if the character is falling or not. This does not take into consideration the HMD.
*		void AdjustPlayerColliderHeight();
*			Changes the player collider's height so that its bottom is in the same location but its top
*			grows or shrinks by the amount the HMD moved this frame.
*		void AdjustPlayerColliderLocation();
*			Moves the player collider so that the top remains at the location of the HMD.
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
*		UVRCameraComponent * m_Camera
*			Camera used to display vision to the HMD.
*		UStaticMesh * LeftHandMesh
*			The mesh used by the left hand.
*		UStaticMesh * RightHandMesh
*			The mesh used by the right hand.
*		UUserWidget * m_FOVRestrictorWidget
*			Used to restrict the users peripheral vision when performing actions that could result in
*			motion sickness.
*		UWidgetComponent * m_FOVRestrictorComponent
*			Contains the widget used to restrict field of vision when performing actions that could cause
*			simulation sickness.
*		UCapsuleComponet * m_playerCollider
*			Capsule component used to experience physics and collisions.
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

	UMotionControllerComponent * GetLeftMotionController();
	UMotionControllerComponent * GetRightMotionController();
	UCapsuleComponent * GetPlayerCollider();

	void MovePlayerColliderWithCamera(float DeltaTime, FMinimalViewInfo& DesiredView);
	void CopyCameraXYToPlayerCollider();
	void ManageCrouchAndJump();
	bool IsFalling();
	void AdjustPlayerColliderHeight();
	void AdjustPlayerColliderLocation();

	UPROPERTY(BlueprintReadWrite, Category = "Appearance")
		UStaticMesh * leftHandMesh;
	UPROPERTY(BlueprintReadWrite, Category = "Appearance")
		UStaticMesh * rightHandMesh;
	UPROPERTY(BlueprintReadWrite, Category = "Appearance")
		UUserWidget * FOVRestrictorWidget;
	UPROPERTY(BlueprintReadOnly, Category = "VR")
		float playerHeight;

private:
	UMotionControllerComponent * m_Controller1;
	UMotionControllerComponent * m_Controller2;		
	UMotionControllerComponent * m_LeftController;
	UMotionControllerComponent * m_RightController;
	UCapsuleComponent * m_PlayerCollider;
	UVRCameraComponent * m_Camera;
	UWidgetComponent * m_FOVRestrictorComponent;
};
