// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "VRPawn.h"
#include "VRPlayerController.generated.h"

/**********************************************************************************************************
*	Class: AVRPlayerController
*
*	Overview:
*		Abstract player controller that contains all functionality required to control a VR pawn.
*		
*
*	Methods:
*		virtual void SetupInputComponent() override
*			Binds inputs from the player's motion controllers to movement in game.
*		virtual void PlayerTick(float DeltaTime) override
*			Each tick calls the locomotion function assigned to move the pawn.
*		void LocomotionWalkForward()
*			Locomotion function. Walks the player forward from their initial direction when the button
*			was pressed. The player will continue to walk forward until the button is released.
*		virtual void RestrictVision()
*			Restricts the player's vision using the FOV Restrictor widget. The specifics of this are
*			implemented by child classes.
*		virtual void UnrestrictVision()
*			Frees the players vision using the FOV Restrictor widget. The specifics of this are
*			implemented by child classes.
*		void BeginMoveLeft()
*			Fires upon the move button being pressed on the left motion controller.
*		void EndMoveLeft()
*			Fires upon the move button being released on the left motion controller.
*		void BeginMoveRight()
*			Fires upon the move button being pressed on the right motion controller.
*		void EndMoveRight()
*			Fires upon the move button being released on the right motion controller.
*
*	Data Members:
*		TSubclassOf<class UUserWidget> FOVRestrictorType
*			Type of widget used to restrict the player's vision during actions prone to cause player motion
*			sickness.
*		UUserWidget * FOVRestrictor
*			Reference to widget used to restrict player vision.
*		bool m_LeftMovePressed
*			Is the left motion controller's move button held down?
*		bool m_RightMovePressed
*			Is the right motion controller's move button held down?
*		void(*m_LocomotionFunction)()
*			Function used for locomotion. This gets called each tick to update the position of the player
*			pawn.
**********************************************************************************************************/
UCLASS(abstract)
class HALVA_API AVRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AVRPlayerController();

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

	void LocomotionWalkForward(float DeltaTime);

	// These functions should be overridden to implement intended behavior.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Appearance")
		void RestrictVision();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Appearance")
		void UnrestrictVision();

	void BeginMoveLeft();
	void EndMoveLeft();
	void BeginMoveRight();
	void EndMoveRight();

	// Type of widget to use for the vision restrictor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		TSubclassOf<class UUserWidget> FOVRestrictorType;

	UPROPERTY(BlueprintReadOnly, Category = "Appearance")
		UUserWidget* FOVRestrictor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
		float maxObscuration;

	// Override BeginPlay()
	virtual void BeginPlay() override;
	
private:
	bool m_LeftMovePressed;
	bool m_RightMovePressed;
	void(AVRPlayerController::*m_LocomotionFunction)(float);
};
