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
			//initialDirection = player->GetVRCameraCapsule()->GetForwardVector();
			RestrictVision();
		}
		// Move the player.
		player->AddActorLocalOffset(initialDirection * DeltaTime * 150);
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