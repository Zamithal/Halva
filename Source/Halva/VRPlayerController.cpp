// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "Blueprint/UserWidget.h"
#include "VRPlayerController.h"

/**********************************************************************************************************
*	DungeonLayout()
*		Purpose:	Constructor.
**********************************************************************************************************/
AVRPlayerController::AVRPlayerController()
{
	m_LeftMovePressed = false;
	m_RightMovePressed = false;
	m_LocomotionFunction = nullptr;
}

// Called when the game starts or when spawned
void AVRPlayerController::BeginPlay()
{
	m_LocomotionFunction = &AVRPlayerController::LocomotionWalkForward;

	// if the class was assigned.
	if (FOVRestrictorType)
	{
		// Attempt to create the widget
		FOVRestrictor = CreateWidget<UUserWidget>(this, FOVRestrictorType);
		FOVRestrictor->SetVisibility(ESlateVisibility::Visible);

		if (FOVRestrictor)
		{
			// get the player pawn.
			AVRPawn * playerPawn = Cast<AVRPawn>(GetPawn());

			if (playerPawn)
			{
				playerPawn->FOVRestrictorWidget = FOVRestrictor;
				playerPawn->Controller = this;
				UCapsuleComponent * playerCollider = playerPawn->GetPlayerCollider();
			}
		}
	}

	Super::BeginPlay();
}
/**********************************************************************************************************
*	void SetupInputComponent()
*		Purpose:	Binds inputs to the player pawn. These are common actions such as walking.
**********************************************************************************************************/
void AVRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Move.L", IE_Pressed, this, &AVRPlayerController::BeginMoveLeft);
	InputComponent->BindAction("Move.L", IE_Released, this, &AVRPlayerController::EndMoveLeft);
	InputComponent->BindAction("Move.R", IE_Pressed, this, &AVRPlayerController::BeginMoveRight);
	InputComponent->BindAction("Move.R", IE_Released, this, &AVRPlayerController::EndMoveRight);

    InputComponent->BindAction("Recalibrate", IE_Pressed, this, &AVRPlayerController::Recalibrate);
}
/**********************************************************************************************************
*	void PlayerTick(float DeltaTime)
*		Purpose:	Triggers once per tick. Updates position of the player dependent on player input and
*					locomotion function.
*		Parameters:
*			float DeltaTime
*				Time since last tick.
*		Changes:
*			The position of the pawn will be changed depending on what locomotion method is selected.
**********************************************************************************************************/
void AVRPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// If movement is enabled.
	if (m_LocomotionFunction != nullptr)
		(this->*m_LocomotionFunction)(DeltaTime);
}
/**********************************************************************************************************
*	void LocomotionWalkForward(float DeltaTime)
*		Purpose:	Locomotion type. Causes the player pawn to move forward from the initial direction
*					facing when the move button was pressed. The pawn will continue to move in the same
*					direction until both move buttons are released. While moving, player peripheral vision
*					is obscured to reduce simulation sickness.
*		Parameters:
*			float DeltaTime
*				Time since last tick.
*		Changes:
*			Position of the pawn is changed.
**********************************************************************************************************/
void AVRPlayerController::LocomotionWalkForward(float DeltaTime)
{
	static bool firstRun = true;

	if (m_LeftMovePressed || m_RightMovePressed)
	{
		static FVector initialDirection = FVector(0, 0, 0);
		static AVRPawn * player;
		// If the button was just pressed find initial direction.
		if (firstRun)
		{
			player = Cast<AVRPawn>(GetPawn());
			firstRun = false;
			initialDirection = player->GetPlayerForwardArrow()->GetForwardVector();
			initialDirection.Z = .1;
			RestrictVision();
		}
		// Move the player.
		player->AddActorLocalOffset(initialDirection * DeltaTime * 150, true);
	}
	else
	{
		if (firstRun == false)
		{
			firstRun = true;
			UnrestrictVision();
		}
	}
}

/**********************************************************************************************************
*	void BeginMoveLeft()
*		Purpose:	Triggers when the move button begins to be pressed on the left motion controller.
*
*		Changes:
*			m_LeftMovePressed - Set to true.
**********************************************************************************************************/
void AVRPlayerController::BeginMoveLeft()
{
	m_LeftMovePressed = true;
}
/**********************************************************************************************************
*	void EndMoveLeft()
*		Purpose:	Triggers when the move button is released on the left motion controller.
*
*		Changes:
*			m_LeftMovePressed - Set to false.
**********************************************************************************************************/
void AVRPlayerController::EndMoveLeft()
{
	m_LeftMovePressed = false;
}
/**********************************************************************************************************
*	void BeginMoveRight()
*		Purpose:	Triggers when the move button begins to be pressed on the right motion controller.
*
*		Changes:
*			m_RightMovePressed - Set to true.
**********************************************************************************************************/
void AVRPlayerController::BeginMoveRight()
{
	m_RightMovePressed = true;
}
/**********************************************************************************************************
*	void EndMoveRight()
*		Purpose:	Triggers when the move button is released on the right motion controller.
*
*		Changes:
*			m_RightMovePressed - Set to false.
**********************************************************************************************************/
void AVRPlayerController::EndMoveRight()
{
	m_RightMovePressed = false;
}

/**********************************************************************************************************
*    void Recalibrate()
*        Purpose:    Triggers when the player presses the Recalibrate button. Adjusts the player's height
*                    and location so that they are currently standing at their full height.
*
*        Changes:
*            playerHeight - Set to the current height of the HMD. This will cause the player to fall back
*                           onto the floor and appear to be the correct height.
**********************************************************************************************************/
void AVRPlayerController::Recalibrate()
{
    static AVRPawn * player = Cast<AVRPawn>(GetPawn());
    
    if (player != nullptr && player->GetPlayerCamera() != nullptr && player->GetPlayerCamera()->bCouldGetHmd)
    {
        player->playerHeight = player->GetPlayerCamera()->GetHmdLocation().Z;
        player->GetPlayerCollider()->SetCapsuleHalfHeight(player->playerHeight / 2);
    }
}
/**********************************************************************************************************
*    void HoverOverObject(UMotionControllerComponent * TriggeringHand)
*        Purpose:    This function fires each time an object overlaps with the motion controller's grab
*                    collider. The overlapping object is checked to see if it implements the usable
*                    interface. If it does, that object is marked as the highlighted object for the given
*                    hand and the object's hover event is fired.
*
*        Parameters:
*            UMotionControllerComponent * TriggeringHand
*                Indicates which hand performed the hover operation.
*
*        Changes:
*            AActor m_LeftHoveredObject - If the triggering hand is the left one and this was a usable
*                                         object, it is set as the hovered object.
*			 AActor m_RightHoverObject - If the triggering hand is the right one and this was a usable
*										 object, it is set as the hovered object.
**********************************************************************************************************/
void AVRPlayerController::HoverOverObject(UMotionControllerComponent * TriggeringHand)
{

    return void;
}