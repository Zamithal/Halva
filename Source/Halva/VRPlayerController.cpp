// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "Blueprint/UserWidget.h"
#include "VRPlayerController.h"
#include "UseInterface.h"
#include "GrabableItem.h"

/**********************************************************************************************************
*	AVRPlayerController()
*		Purpose:	Constructor.
**********************************************************************************************************/
AVRPlayerController::AVRPlayerController()
{
	m_LeftMovePressed = false;
	m_RightMovePressed = false;
	m_LocomotionFunction = nullptr;
	m_SinglePulseHapticEffect = nullptr;
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

	Recalibrate();

	m_player = Cast<AVRPawn>(GetPawn());


	m_player->GetLeftHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObject);
	m_player->GetLeftHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObject);
	m_player->GetRightHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObject);
	m_player->GetRightHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObject);
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
	static bool visionRestricted = false;

	if (m_LeftMovePressed || m_RightMovePressed)
	{
		static FVector direction = FVector(0, 0, 0);
		// If the button was just pressed find initial direction.
		if (visionRestricted == false)
		{
			visionRestricted = true;
			direction = m_player->GetPlayerStaticForwardArrow()->GetForwardVector();
			direction.Z = .1;
			RestrictVision();
		}
		// Move the player.
		m_player->AddActorLocalOffset(direction * DeltaTime * 150, true);
	}
	else
	{
		if (visionRestricted == true)
		{
			visionRestricted = false;
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
    
    if (m_player != nullptr && m_player->GetPlayerCamera() != nullptr && m_player->GetPlayerCamera()->bCouldGetHmd)
    {
		m_player->playerHeight = m_player->GetPlayerCamera()->GetHmdLocation().Z;
		m_player->GetPlayerCollider()->SetCapsuleHalfHeight(m_player->playerHeight / 2);

		m_player->GetPlayerStaticForwardArrow()->RelativeLocation.Z = -(m_player->playerHeight / 2);
		m_player->GetPlayerHeightDependentFowardArrow()->RelativeLocation.Z = (m_player->playerHeight / 2);

		// Move 1 player head's below the top of the head. (8 heads in a body height)
		// Subtract the height of the box.
		m_player->GetLeftShoulderBox()->RelativeLocation.Z = -m_player->GetLeftShoulderBox()->Bounds.BoxExtent.Z;
		m_player->GetRightShoulderBox()->RelativeLocation.Z = -m_player->GetRightShoulderBox()->Bounds.BoxExtent.Z;
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
	// The player is doing a sheathing action?
	if (m_player->GetLeftShoulderBox()->IsOverlappingComponent(m_player->GetLeftHandGrabCollider()))
		ManageWeaponSheathe(m_player->GetLeftShoulderBox(), m_player->GetLeftHandGrabCollider(), m_LeftHandObject);
	else if (m_player->GetRightShoulderBox()->IsOverlappingComponent(m_player->GetLeftHandGrabCollider()))
		ManageWeaponSheathe(m_player->GetRightShoulderBox(), m_player->GetLeftHandGrabCollider(), m_LeftHandObject);
	// The player is doing a regular action?
	else if (m_LeftHandObject != nullptr)
	{
		IUseInterface* usableItem = Cast<IUseInterface>(m_LeftHandObject);

		// TODO: Specify Socket
		TArray<FString> params;

		params.Add("Free");
		usableItem->Execute_Use(m_LeftHandObject, m_player->GetLeftMotionController(), params);
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
	// The player is doing a sheathing action?
	if (m_player->GetLeftShoulderBox()->IsOverlappingComponent(m_player->GetRightHandGrabCollider()))
		ManageWeaponSheathe(m_player->GetLeftShoulderBox(), m_player->GetRightHandGrabCollider(), m_RightHandObject);
	else if (m_player->GetRightShoulderBox()->IsOverlappingComponent(m_player->GetRightHandGrabCollider()))
		ManageWeaponSheathe(m_player->GetRightShoulderBox(), m_player->GetRightHandGrabCollider(), m_RightHandObject);
	// The player is doing a regular action?
	if (m_RightHandObject != nullptr)
	{
		IUseInterface* usableItem = Cast<IUseInterface>(m_RightHandObject);

		// TODO: Specify Socket
		TArray<FString> params;
		params.Add("Free");

		usableItem->Execute_Use(m_RightHandObject, m_player->GetRightMotionController(), params);
	}
}
/**********************************************************************************************************
*    void HoverOverObject(...)
*        Purpose:    This function fires each time an object overlaps with the motion controller's grab
*                    collider. The overlapping object is checked for multiple conditions, an action is
*					 performed based on the checks.
*
*        Parameters:
*            These are standard Overlap Parameters.
**********************************************************************************************************/
void AVRPlayerController::HoverOverObject(UPrimitiveComponent* OverlappedComp, AActor* Object, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<IUseInterface>(Object) != nullptr)
		HighlightUsable(Object, OverlappedComp);

	if (OtherComp == m_player->GetLeftShoulderBox() || OtherComp == m_player->GetRightShoulderBox())
		InformShoulderBoxHit(OtherComp, OverlappedComp);
}
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
	if (Cast<IUseInterface>(Object) != nullptr)
		DehighlightUsable(Object, OverlappedComp);
}
/**********************************************************************************************************
*    bool HighlightUsable(AActor* Object, USceneComponent* TriggeringObject)
*        Purpose:    Executes the passed object's highlight function if condition checks are met.
*					 Additionally, assigns the object to the given hand assuming conditions also match.
*
*        Parameters:
*            AActor* Object
*				The usable object to be highlighted.
*			 UPrimitiveComponent* TriggeringObject
*				The component that triggered this call.
*
*        Changes:
*            m_LeftHandObject
*				 May be assigned to.
*			 m_RightHandObject
*				 May be assigned to.
*
*		 Return:
*			 Returns if the object was highlighted or not.
**********************************************************************************************************/
bool AVRPlayerController::HighlightUsable(AActor* Object, UPrimitiveComponent* TriggeringObject)
{
	if (Object == nullptr)
		return false;

	if (TriggeringObject == m_player->GetLeftHandGrabCollider())
	{
		if (m_LeftHandObject != nullptr)
			return false;
	}
	else if (TriggeringObject == m_player->GetRightHandGrabCollider())
	{
		if (m_RightHandObject != nullptr)
			return false;
	}

	IUseInterface* usableObject = Cast<IUseInterface>(Object);
	if (usableObject == nullptr)
		return false;


	usableObject->Execute_Highlight(Object);

	if (TriggeringObject == m_player->GetLeftHandGrabCollider())
	{
		m_LeftHandObject = Object;
		PlayHapticEffect(m_SinglePulseHapticEffect, EControllerHand::Left, .5);
	}
	else if (TriggeringObject == m_player->GetRightHandGrabCollider())
	{
		m_RightHandObject = Object;
		PlayHapticEffect(m_SinglePulseHapticEffect, EControllerHand::Right, .5);
	}

	return true;
}
/**********************************************************************************************************
*    bool DehighlightUsable(AActor* Object, UPrimitiveComponent* TriggeringObject)
*        Purpose:    Dehighlights the given object assuming it passes all checks.
*					 A new object will be assigned to the triggering hand if available.
*
*        Parameters:
*            AActor* Object
*				The usable object to be dehighlighted.
*			 UPrimitiveComponent* TriggeringObject
*				The component that triggered this call.
*
*        Changes:
*            m_LeftHandObject
*				 If the object that triggered is the left hand, the closest other overlapping object will
*				 be assigned to this, if there are no objects it will be set to nullptr.
*            m_RightHandObject
*				 If the object that triggered is the right hand, the closest other overlapping object will
*				 be assigned to this, if there are no objects it will be set to nullptr.
*
*        Return:
*			 Returns if a n object was successfully dehighlighted or not.
**********************************************************************************************************/
bool AVRPlayerController::DehighlightUsable(AActor* Object, UPrimitiveComponent* TriggeringObject)
{
	if (Object == nullptr)
		return false;

	if (TriggeringObject == m_player->GetLeftHandGrabCollider() && Object != m_LeftHandObject)
		return false;

	if (TriggeringObject == m_player->GetRightHandGrabCollider() && Object != m_RightHandObject)
		return false;


	TSet<AActor *> retrievedSet;
	TriggeringObject->GetOverlappingActors(retrievedSet);
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
	FVector handPosition = TriggeringObject->GetComponentLocation();
	float closestObjectDistance = 1000000;
	//Find closest usable.
	for (int i = 0; i < overlaps.Num(); i++)
	{
		float distance = (overlaps[i]->GetActorLocation() - handPosition).Size();
		if (distance < closestObjectDistance)
			closestObject = overlaps[i];
	}


	if (TriggeringObject == m_player->GetLeftHandGrabCollider())
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
	else if (TriggeringObject == m_player->GetRightHandGrabCollider())
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

	return true;
}
/**********************************************************************************************************
*    bool InformShoulderBoxHit(USceneComponent* Object, UPrimitiveComponent* TriggeringObject)
*        Purpose:    Creates a haptic feedback event for the triggering motion controller that entered the
*					 shoulder box.
*
*        Parameters:
*            USceneComponent* ShoulderBox
*				 This should always be one of the shoulder boxes.
*			 UPrimitiveComponent* TriggeringObject
*				 This is one of the grab colliders for a motion controller.
*
*        Return: Returns if an event was played or not.
**********************************************************************************************************/
bool AVRPlayerController::InformShoulderBoxHit(USceneComponent* ShoulderBox, UPrimitiveComponent* TriggeringObject)
{
	if (ShoulderBox == nullptr || TriggeringObject == nullptr)
		return false;

	if (ShoulderBox != m_player->GetLeftShoulderBox() && ShoulderBox != m_player->GetRightShoulderBox())
		return false;

	// Don't play the effect if there is already a sheathed item.
	if (ShoulderBox->GetNumChildrenComponents() > 0)
		return false;

	if (m_SinglePulseHapticEffect != nullptr && TriggeringObject == m_player->GetLeftHandGrabCollider())
		PlayHapticEffect(m_SinglePulseHapticEffect, EControllerHand::Left, 1);
	else if (m_SinglePulseHapticEffect != nullptr && TriggeringObject == m_player->GetRightHandGrabCollider())
		PlayHapticEffect(m_SinglePulseHapticEffect, EControllerHand::Right, 1);
	else
		return false;
	
	return true;
}
/**********************************************************************************************************
*    bool ManageWeaponSheathe(USceneComponent*  ShoulderBox, UPrimitiveComponent* TriggeringObject, AActor* SheathedActor)
*        Purpose:    This function attaches and detaches the given actor to the player's back. The item
*					 must inherit from GrabbableItem.
*
*        Parameters:
*            USceneComponent* ShoulderBox
*				 The shoulder to attach/detach the actor to/from.
*			 UPrimitiveComponent* TriggeringObject
*				 The component that triggered this event. If the actor is being unsheathed it will be
*				 attached to this component.
*			 AActor* SheathedActor
*				 The actor to sheathe/unsheathe.
*
*        Changes:
*            SheathedActor
*				Could be attached to the TriggeringObject or the ShoulderBox.
*
*        Return: Returns if the operation could be completed or not.
**********************************************************************************************************/
bool AVRPlayerController::ManageWeaponSheathe(USceneComponent*  ShoulderBox, UPrimitiveComponent* TriggeringObject, AActor* SheathedActor)
{
	if (ShoulderBox == nullptr || TriggeringObject == nullptr || SheathedActor == nullptr)
		return false;

	AGrabableItem* grabable = Cast<AGrabableItem>(SheathedActor);
	if (grabable == nullptr)
		return false;

	SheathedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Sheathe
	if ((ShoulderBox == m_player->GetLeftShoulderBox() && m_player->GetLeftSheathedItem() == nullptr)
		||
		(ShoulderBox == m_player->GetRightShoulderBox() && m_player->GetRightSheathedItem() == nullptr))
	{	
		SheathedActor->AttachToComponent(ShoulderBox, FAttachmentTransformRules::KeepWorldTransform);
	}
	// Unsheathe
	else if ((ShoulderBox == m_player->GetLeftShoulderBox() && m_player->GetLeftSheathedItem() == SheathedActor)
			||
			(ShoulderBox == m_player->GetRightShoulderBox() && m_player->GetRightSheathedItem() == SheathedActor))
	{
		SheathedActor->AttachToComponent(TriggeringObject, FAttachmentTransformRules::KeepWorldTransform);
	}
	else
		return false;

	return true;
}