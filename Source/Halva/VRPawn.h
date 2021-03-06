// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "MotionControllerComponent.h"
#include "VRCameraCapsuleComponent.h"
#include "VRCameraComponent.h"
#include "PotionInterface.h"
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
*		PlayerCamera
*			-Get
*		PlayerStaticForwardArrow
*			-Get
*		PlayerHeightDependantForwardArrow
*			-Get
*		LeftHandGrabCollider
*			-Get
*		RightHandGrabCollider
*			-Get
*		LeftShoulderBox
*			-Get
*		RightShoulderBox
*			-Get
*
*	Methods:
*		InitHands()
*			Initializes all components relating to the player's hands. Only called by the constructor.
*		InitBody()
*			Initializes all components relating to the player's body. Only called by the constructor.
*		InitVision()
*			Initializes all components relating to the player's vision. Only called by the constructor.
*		virtual void TakeDamage(...)
*			Tells the controller this took damage.
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
*		void AdjustCameraLocation();
*			Moves the camera so that it remains at the top of the player collider.
*		void UpdatePlayerAttachmentPoints();
*			Updates the player's forward arrow so that it is facing the direction of the camera. on the XY
*			XY plane.
*		void AdjustRoomCenterLocation();
*			Changes the center of the room's attachment point to reflect motion the player performed last
*			frame. The change is the inverse of any motion the player performed in order to keep the center
*			still.
*		AActor * GetLeftSheathedItem();
*			Returns the item in the left sheathe on the player's back. Returns nullptr if there is no item.
*		AActor * GetRightSheathedItem();
*			Returns the item in the right sheathe on the player's back. Returns nullptr if there is no item.
*		void ModifyHealth(float Amount)
*			Passes the call to modify health to the player controller.
*
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
*		USphereComponent * m_LeftHandGrabCollider
*			Pointer to the collider used to indicate the position of the left hand.
*		USphereComponent * m_rightHandGrabCollider
*			Pointer to the collider used to indicate the position of the right hand.
*		UVRCameraComponent * m_Camera
*			Camera used to display vision to the HMD.
*		UStaticMesh * LeftHandMesh
*			The mesh used by the left hand.
*		UStaticMesh * RightHandMesh
*			The mesh used by the right hand.
*		UUserWidget * m_FOVRestrictorWidget
*			Used to restrict the users peripheral vision when performing actions that could result in
*			motion sickness.
*		float MaximumHealth
*			The Maximum health of the player.
*		UWidgetComponent* HealthBarWidgetComponent
*			The health bar of the player.
*		UWidgetComponent * m_FOVRestrictorComponent
*			Contains the widget used to restrict field of vision when performing actions that could cause
*			simulation sickness.
*		UCapsuleComponet * m_playerCollider
*			Capsule component used to experience physics and collisions.
*		UArrowComponent * m_StaticForwardAttachmentPoint
*			Used to Attach things to the player. Things attached here will mimic the rotation of the and
*			position of the camera but will not rotate on the X or Y axis. These objects will not move up
*			or down with the camera.
*		UArrowComponent * m_HeightDependantForwardAttachmentPoint
*			Functions similarly to the static attachment point, except moves up and down along the Z axis
*			to accommodate the player's height.
*		USceneComponent * m_RoomAttachmentPoint
*			Used for things that should not be influenced by the camera's location and rotation. Things
*			attached to this do not move according to the camera in roomscale. Mostly used for other
*			roomscale objects such as motion controllers.
*		UBoxComponent * m_LeftShoulderBox
*			A box used for sheathing weapons. Dropping an item on this will attach it to your back.
*		UBoxComponent * m_RightShoulderBox
*			A box used for sheathing weapons. Dropping an item on this will attach it to your back.
**********************************************************************************************************/
UCLASS()
class HALVA_API AVRPawn : public APawn, public IPotionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();
	void OnConstruction(const FTransform& Transform) override;

	void InitHands();
	void InitBody();
	void InitVision();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Called when taking damage.
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UMotionControllerComponent * GetLeftMotionController();
	UMotionControllerComponent * GetRightMotionController();
	USphereComponent * GetLeftHandGrabCollider();
	USphereComponent * GetRightHandGrabCollider();
	UCapsuleComponent * GetPlayerCollider();
	UVRCameraComponent * GetPlayerCamera();
	UArrowComponent * GetPlayerStaticForwardArrow();
	UArrowComponent * GetPlayerHeightDependentFowardArrow();
	UBoxComponent * GetLeftShoulderBox();
	UBoxComponent * GetRightShoulderBox();

	AActor* GetLeftSheathedItem();
	AActor* GetRightSheathedItem();

	void UpdateRoomscaleComponents(float DeltaTime, FMinimalViewInfo& DesiredView);
	void MovePlayerColliderWithCamera(float DeltaTime, FMinimalViewInfo& DesiredView);
	void CopyCameraXYToPlayerCollider();
	void ManageCrouchAndJump();
	bool IsFalling();
	void AdjustPlayerColliderHeight();
	void AdjustPlayerColliderLocation();
	void AdjustCameraLocation();
	void UpdatePlayerAttachmentPoints();
	void UpdateRoomAttachmentPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float MaximumHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UWidgetComponent* HealthBarWidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR")
		UWidgetComponent* FOVRestrictorComponent;
	UPROPERTY(BlueprintReadOnly, Category = "VR")
		float playerHeight;

	// PotionInterface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Potion")
		void ModifyHealth(float Amount);
	virtual void ModifyHealth_Implementation(float Amount) override;

private:

	UMotionControllerComponent * m_Controller1;
	UMotionControllerComponent * m_Controller2;		
	UMotionControllerComponent * m_LeftController;
	UMotionControllerComponent * m_RightController;

	USphereComponent * m_LeftHandGrabCollider;
	USphereComponent * m_RightHandGrabCollider;
	UBoxComponent * m_LeftShoulderBox;
	UBoxComponent * m_RightShoulderBox;

	UCapsuleComponent * m_PlayerCollider;
	UVRCameraComponent * m_Camera;
	UArrowComponent * m_StaticForwardAttachmentPoint;
	UArrowComponent * m_HeightDependantForwardAttachmentPoint;
	USceneComponent * m_RoomAttachmentPoint;


};
