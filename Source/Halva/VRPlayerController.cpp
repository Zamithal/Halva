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


	player->GetLeftHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObject);
	player->GetLeftHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObject);
	player->GetRightHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObject);
	player->GetRightHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObject);
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
*    void HoverOverObject(...)
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
void AVRPlayerController::HoverOverObject(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString name = OverlappedComp->GetName();

	if (OverlappedComp->GetName() == "LH Grab Collider")
	{
		if (m_LeftHandObject != nullptr)
			return;
	}
	else
	{
		if (m_RightHandObject != nullptr)
			return;
	}
	
	if (Object == nullptr)
		return;

	IUseInterface* usableObject = Cast<IUseInterface>(Object);
	if (usableObject == nullptr)
		return;

	static AVRPawn * player = Cast<AVRPawn>(GetPawn());

	usableObject->Execute_Highlight(Object);

	if (OverlappedComp->GetName() == "LH Grab Collider")
		m_LeftHandObject = Object;
	else
		m_RightHandObject = Object;
}
// Outdated method.
//void AVRPlayerController::HoverOverObject(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (Object == nullptr || Object == m_LeftHandObject)
//		return;
//
//	IUseInterface* useableObject = Cast<IUseInterface>(Object);
//	if (useableObject == nullptr)
//		return;
//
//	static AVRPawn * player = Cast<AVRPawn>(GetPawn());
//
//	if (player == nullptr)
//		return;
//
//	IUseInterface* oldObject = nullptr;
//
//	if (m_LeftHandObject != nullptr)
//		oldObject = Cast<IUseInterface>(m_LeftHandObject);
//
//	// If the old object has no hands hovering over it.
//	if (m_LeftHandObject != m_RightHandObject)
//		oldObject->Execute_Dehighlight(m_LeftHandObject);
//
//	// If the new object is not already highlighted.
//	if (m_LeftHandObject != Object && m_RightHandObject != Object)
//		useableObject->Execute_Highlight(Object);
//
//	m_LeftHandObject = Object;
//}
/**********************************************************************************************************
*    void EndHoverOverObject(...)
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
void AVRPlayerController::EndHoverOverObject(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComp->GetName() == "LH Grab Collider" && Object != m_LeftHandObject)
		return;

	if (OverlappedComp->GetName() == "RH Grab Collider" && Object != m_RightHandObject)
		return;


	TSet<AActor *> retrievedSet;
	OverlappedComp->GetOverlappingActors(retrievedSet);
	TArray<AActor *> overlaps = retrievedSet.Array();


	// Find all usables.
	for (int i = 0; i < overlaps.Num(); i++)
	{
		IUseInterface* usable = Cast<IUseInterface>(overlaps[i]);
		if (usable == nullptr)
		{
			overlaps.RemoveAt(i);
			i--;
		}
	}

	AActor * closestObject = nullptr;
	FVector handPosition = OverlappedComp->GetComponentLocation();
	float closestObjectDistance = 1000000;
	//Find closest usable.
	for (int i = 0; i < overlaps.Num(); i++)
	{
		float distance = (overlaps[i]->GetActorLocation() - handPosition).Size();
		if (distance < closestObjectDistance)
			closestObject = overlaps[i];
	}


	if (OverlappedComp->GetName() == "LH Grab Collider")
	{
		if (m_LeftHandObject != nullptr)
			Cast<IUseInterface>(m_LeftHandObject)->Execute_Dehighlight(m_LeftHandObject);

		if (closestObject != nullptr)
		{
			m_LeftHandObject = closestObject;
			Cast<IUseInterface>(closestObject)->Execute_Highlight(closestObject);
		}
		else
			m_LeftHandObject = nullptr;
	}
	else
	{
		if (m_RightHandObject != nullptr)
			Cast<IUseInterface>(m_RightHandObject)->Execute_Dehighlight(m_RightHandObject);

		if (closestObject != nullptr)
		{
			m_RightHandObject = closestObject;
			Cast<IUseInterface>(closestObject)->Execute_Highlight(closestObject);
		}
		else
			m_RightHandObject = nullptr;
	}

}