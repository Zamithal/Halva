// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "MotionControllerComponent.h"
#include "SteamVRChaperoneComponent.h"
#include "Components/WidgetComponent.h"
#include "ViveMovementComponent.h"
#include "ChildTransformConsumerComponent.h"
#include "GameFramework/Pawn.h"
#include "PlayerVive.generated.h"

/**********************************************************************************************************
*	Class: PlayerVive
*
*	Overview:
*		This pawn is used to control the player character. This class is for use with the HTC Vive and
*		uses functionality specific to the Vive. This class interfaces with the users head mounted display
*		and motion controllers. Additionally, this class contains methods for interacting with other actors
*		in the game.
*
*	Methods:
*		SetMotionControllerSides()
*			Assigns the motion controller that is left of the other one to LeftController. If the motion
*			controller has a sceneComponent that needs to be attached to it, it is reattached. The same
*			logic is held for RightController.
*		SetPlayerHeight()
*			Gets the players current height and stores it. This assumes the player is standing up straight.

*	Data Members:
*		USteamVRChaperoneComponent * m_Chaperone
*			Shows the edges of the real world room to the user in the head mounted display.
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
*		UWidgetComponent * m_FOVRestrictor
*			Used to restrict the users peripheral vision when performing actions that could result in
*			motion sickness.
*		UCapsuleComponent * m_GroundCollider
*			Used to solve basic collision on the player. This is attached to the camera in such a way that
*			does not allow rotation.
*		UPawnMovementComponent * m_MovementComponent
*			Allows the player to move and experience physics.
**********************************************************************************************************/
UCLASS()
class HALVA_API APlayerVive : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerVive();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		UStaticMesh * LeftHandMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		UStaticMesh * RightHandMesh;
	
private:
	USteamVRChaperoneComponent * m_Chaperone;
	UMotionControllerComponent * m_Controller1;
	UMotionControllerComponent * m_Controller2;
	UMotionControllerComponent * m_LeftController;
	UMotionControllerComponent * m_RighController;
	UCameraComponent * m_Camera;
	UWidgetComponent * m_FOVRestrictor;
	UCapsuleComponent * m_GroundCollider;

	class UViveMovementComponent * m_MovementComponent;
};
