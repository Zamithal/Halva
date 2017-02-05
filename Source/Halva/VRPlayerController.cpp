// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "Blueprint/UserWidget.h"
#include "VRPlayerController.h"
#include "UseInterface.h"

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
	Super::BeginPlay();

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

			if (playerPawn != nullptr)
			{
				playerPawn->FOVRestrictorWidget = FOVRestrictor;
				playerPawn->Controller = this;
				UCapsuleComponent * playerCollider = playerPawn->GetPlayerCollider();
			}
		}
	}

	AVRPawn * player = Cast<AVRPawn>(GetPawn());


	player->GetLeftHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObjectLeft);
	player->GetLeftHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObjectLeft);
	player->GetRightHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObjectRight);
	player->GetRightHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObjectRight);
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

	InputComponent->BindAction("Grab.L", IE_Pressed, this, &AVRPlayerController::UseItemLeft);
	InputComponent->BindAction("Grab.R", IE_Pressed, this, &AVRPlayerController::UseItemRight);

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
    AVRPawn * player = Cast<AVRPawn>(GetPawn());
    
    if (player != nullptr && player->GetPlayerCamera() != nullptr && player->GetPlayerCamera()->bCouldGetHmd)
    {
        player->playerHeight = player->GetPlayerCamera()->GetHmdLocation().Z;
        player->GetPlayerCollider()->SetCapsuleHalfHeight(player->playerHeight / 2);
    }
}
/**********************************************************************************************************
*    void UseItemLeft()
*        Purpose:    This function fires when the user clicks the grab button. If the left object is valid
*					 and usable, it's use function is executed.
*
*        Changes:
*            m_LeftHandObject
*				 Use function is executed.
**********************************************************************************************************/
void AVRPlayerController::UseItemLeft()
{
	AVRPawn * player = Cast<AVRPawn>(GetPawn());

	if (m_LeftHandObject != nullptr)
	{
		IUseInterface* usableItem = Cast<IUseInterface>(m_LeftHandObject);

		// TODO: Specify Socket
		TArray<FString> params;
		usableItem->Execute_Use(m_LeftHandObject, player->GetLeftMotionController(), params);
	}
}
/**********************************************************************************************************
*    void HoverOverObjectLeft(...)
*        Purpose:    This function fires each time an object overlaps with the motion controller's grab
*                    collider. The overlapping object is checked to see if it implements the usable
*                    interface. If it does, that object is marked as the highlighted object for the given
*                    hand and the object's hover event is fired.
*
*        Parameters:
*            These are standard Overlap Parameters.
*
*        Changes:
*            AActor m_LeftHandObject - If the object is useable, this is replaced by the object.
**********************************************************************************************************/
void AVRPlayerController::HoverOverObjectLeft(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Object == nullptr || Object == m_LeftHandObject)
		return;

	IUseInterface* useableObject = Cast<IUseInterface>(Object);
	if (useableObject == nullptr)
		return;

	static AVRPawn * player = Cast<AVRPawn>(GetPawn());

	if (player == nullptr)
		return;

	IUseInterface* oldObject = nullptr;

	if (m_LeftHandObject != nullptr)
		oldObject = Cast<IUseInterface>(m_LeftHandObject);

	// If the old object has no hands hovering over it.
	if (m_LeftHandObject != m_RightHandObject)
		oldObject->Execute_Dehighlight(m_LeftHandObject);

	// If the new object is not already highlighted.
	if (m_LeftHandObject != Object && m_RightHandObject != Object)
		useableObject->Execute_Highlight(Object);

	m_LeftHandObject = Object;
}
/**********************************************************************************************************
*    void EndHoverOverObjectLeft(...)
*        Purpose:    Fires when the player stops hovering over an object. The object is checked to see if
*					 it is a highlightable object. If it is, and the other hand is not hovering over it,
*					 the object becomes dehighlighted.
*
*        Parameters:
*            These are standard EndOverlap parameters.
*
*        Changes:
*            m_LeftHoverObject
*				 If the object that is no longer hovered over is this, this is set to nullptr.
**********************************************************************************************************/
void AVRPlayerController::EndHoverOverObjectLeft(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVRPawn * player = Cast<AVRPawn>(GetPawn());

	// If the object is highlighted by this hand
	if (Object == m_LeftHandObject)
	{
		// If the object is not highlighted by the other hand.
		if (Object != m_RightHandObject)
		{
			IUseInterface* usableObject = Cast<IUseInterface>(m_LeftHandObject);
			usableObject->Execute_Dehighlight(m_LeftHandObject);
		}

		m_LeftHandObject = nullptr;
	}
}
/**********************************************************************************************************
*    void UseItemRight()
*        Purpose:    This function fires when the user clicks the grab button. If the right object is valid
*					 and usable, it's use function is executed.
*
*        Changes:
*            m_LeftHandObject
*				 Use function is executed.
**********************************************************************************************************/
void AVRPlayerController::UseItemRight()
{
	AVRPawn * player = Cast<AVRPawn>(GetPawn());

	if (m_RightHandObject != nullptr)
	{
		IUseInterface* usableItem = Cast<IUseInterface>(m_RightHandObject);

		// TODO: Specify Socket
		TArray<FString> params;
		usableItem->Execute_Use(m_RightHandObject, player->GetRightMotionController(), params);
	}
}
/**********************************************************************************************************
*    void HoverOverObjectRight(...)
*        Purpose:    This function fires each time an object overlaps with the motion controller's grab
*                    collider. The overlapping object is checked to see if it implements the usable
*                    interface. If it does, that object is marked as the highlighted object for the given
*                    hand and the object's hover event is fired.
*
*        Parameters:
*            These are standard Overlap parameters.
*
*        Changes:
*			 AActor m_RightHoverObject - If the object is useable, this is replaced by the object.
**********************************************************************************************************/
void AVRPlayerController::HoverOverObjectRight(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Object == nullptr || Object == m_RightHandObject)
		return;

	IUseInterface* useableObject = Cast<IUseInterface>(Object);
	if (useableObject == nullptr)
		return;

	static AVRPawn * player = Cast<AVRPawn>(GetPawn());

	if (player == nullptr)
		return;

	IUseInterface* oldObject = nullptr;

	if (m_RightHandObject != nullptr)
		oldObject = Cast<IUseInterface>(m_RightHandObject);
	
	// If the old object has no hands hovering over it.
	if (m_RightHandObject != m_LeftHandObject)
		oldObject->Execute_Dehighlight(m_RightHandObject);

	// If the new object is not already highlighted.
	if (m_RightHandObject != Object && m_LeftHandObject != Object)
		useableObject->Execute_Highlight(Object);

	m_RightHandObject = Object;
}
/**********************************************************************************************************
*    void EndHoverOverObjectRight(...)
*        Purpose:    Fires when the player stops hovering over an object. The object is checked to see if
*					 it is a highlightable object. If it is, and the other hand is not hovering over it,
*					 the object becomes dehighlighted.
*
*        Parameters:
*            These are standard EndOverlap parameters.
*
*        Changes:
*            m_RightHoverObject
*				 If the object that is no longer hovered over is this, this is set to nullptr.
**********************************************************************************************************/
void AVRPlayerController::EndHoverOverObjectRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVRPawn * player = Cast<AVRPawn>(GetPawn());

	// If the object is highlighted by this hand
	if (OtherActor == m_RightHandObject)
	{
		// If the object is not highlighted by the other hand.
		if (OtherActor != m_LeftHandObject)
		{
			IUseInterface* usableObject = Cast<IUseInterface>(m_RightHandObject);
			usableObject->Execute_Dehighlight(m_RightHandObject);
		}

		m_RightHandObject = nullptr;
	}
}