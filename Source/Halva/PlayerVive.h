// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "MotionControllerComponent.h"
#include "SteamVRChaperoneComponent.h"
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
*	UE4 Functions:
*
*	Mutators:
*
*	Methods:
*
*	Data Members:
*		
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

	
private:
	// Chaperone needs forward declaration.
	class USteamVRChaparoneComponent * m_Chaparone;
	UMotionControllerComponent * m_Controller1;
	UMotionControllerComponent * m_Controller2;
	UMotionControllerComponent * m_LeftController;
	UMotionControllerComponent * m_RighController;
	UCameraComponent * m_Camera;
};
