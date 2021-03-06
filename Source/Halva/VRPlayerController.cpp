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

}
/**********************************************************************************************************
*	void OnConstruction(const FTransform& Transform)
*		Purpose:	UE4 Construction Script.
**********************************************************************************************************/
void AVRPlayerController::OnConstruction(const FTransform& Transform)
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
}
/**********************************************************************************************************
*    void Possess(class APawn* Pawn)
*        Purpose:    Fires whenever this AI takes control of a pawn.
*
*        Parameters:
*            class APawn* Pawn
*				 The type of pawn being controlled.
**********************************************************************************************************/
void AVRPlayerController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	m_player = Cast<AVRPawn>(InPawn);

	m_LocomotionFunction = &AVRPlayerController::LocomotionWalkForward;

	m_FOVRestrictorType = m_player->FOVRestrictorComponent->GetWidgetClass();
	m_FOVRestrictor = m_player->FOVRestrictorComponent->GetUserWidgetObject();

	// Should be unnecessary
	//m_player->Controller = this;

	UCapsuleComponent * playerCollider = m_player->GetPlayerCollider();

	Recalibrate();

	if (m_player->HealthBarWidgetComponent != nullptr)
		m_healthBar = Cast<UProgressBarWidget>(m_player->HealthBarWidgetComponent->GetUserWidgetObject());

	if (m_healthBar != nullptr)
		m_healthBar->SetFilledPercentage(1);
	
	CurrentHealth = m_player->MaximumHealth;

	m_player->GetLeftHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObject);
	m_player->GetLeftHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObject);
	m_player->GetRightHandGrabCollider()->OnComponentBeginOverlap.AddDynamic(this, &AVRPlayerController::HoverOverObject);
	m_player->GetRightHandGrabCollider()->OnComponentEndOverlap.AddDynamic(this, &AVRPlayerController::EndHoverOverObject);

	OnTakeAnyDamage.AddDynamic(this, &AVRPlayerController::OnDamageHit);
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

	InputComponent->BindAction("Grab.L", IE_Released, this, &AVRPlayerController::GrabPressedLeft);
	InputComponent->BindAction("Grab.R", IE_Released, this, &AVRPlayerController::GrabPressedRight);

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
			RestrictVision(m_FOVRestrictor);
		}
		// Move the player.
		m_player->AddActorLocalOffset(direction * DeltaTime * 150, true);
	}
	else
	{
		if (visionRestricted == true)
		{
			visionRestricted = false;
			UnrestrictVision(m_FOVRestrictor);
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
*    void GrabPressedLeft()
*        Purpose:    This function calls the use function for the left hand.
**********************************************************************************************************/
void AVRPlayerController::GrabPressedLeft()
{
	GrabPressed(m_player->GetLeftHandGrabCollider(), m_LeftHandObject);
}
/**********************************************************************************************************
*    void GrabPressedRight()
*        Purpose:    This function calls the use function for the right hand.
**********************************************************************************************************/
void AVRPlayerController::GrabPressedRight()
{
	GrabPressed(m_player->GetRightHandGrabCollider(), m_RightHandObject);
}
/**********************************************************************************************************
*    void GrabPressed(UPrimitiveComponent* HandCollider, AActor* GrabbedObject)
*        Purpose:    This function executes logic for when the grab button is pressed. depending on when
*					 and where the button is pressed, this can create different behaviors but typically
*					 leads to the passed actor being grabbed.
*
*        Parameters:
*            UPrimitiveComponent* HandCollider
*				 The collider for the hand that is doing the manipulation.
*			 AActor* GrabbedObject
*				 The object that is being manipulated.
**********************************************************************************************************/
void AVRPlayerController::GrabPressed(UPrimitiveComponent* Hand, AActor* GrabbedObject)
{

	// The player is sheathing their weapon?
	if (Hand->IsOverlappingComponent(m_player->GetLeftShoulderBox()) && m_player->GetLeftSheathedItem() == nullptr)
		SheatheItem(m_player->GetLeftShoulderBox(), GrabbedObject);
	else if (Hand->IsOverlappingComponent(m_player->GetRightShoulderBox()) && m_player->GetRightSheathedItem() == nullptr)
		SheatheItem(m_player->GetRightShoulderBox(), GrabbedObject);
	// The player is doing a regular action?
	else if (GrabbedObject != nullptr)
	{
		if (GrabbedObject == m_player->GetLeftSheathedItem() || GrabbedObject == m_player->GetRightSheathedItem())
			GrabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		IUseInterface* usableItem = Cast<IUseInterface>(GrabbedObject);

		TArray<FString> params;

		params.Add("Free");
		usableItem->Execute_Use(GrabbedObject, Hand, params);

		
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
*    void OnDamagingHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
*        Purpose:    Causes this enemy to take damage. The amount of damage may differ from the amount
*					 passed in depending on if the enemy is resistant to the damage.
*
*        Parameters:
*            Standard OnTakeAnyDamage parameters.
*
*        Changes:
*            CurrentHealth
*				 The current health of this enemy will be decreased.
**********************************************************************************************************/
void AVRPlayerController::OnDamageHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;

	if (m_healthBar != nullptr && m_player->MaximumHealth != 0)
		m_healthBar->SetFilledPercentage(CurrentHealth / m_player->MaximumHealth);
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
*    bool SheatheItem(USceneComponent*  ShoulderBox, UPrimitiveComponent* TriggeringObject, AActor* SheathedActor)
*        Purpose:    This function attaches the given actor to the given shoulder box.
*
*        Parameters:
*            USceneComponent* ShoulderBox
*				 The shoulder to attach to.
*			 AActor* SheathedActor
*				 The actor to sheathe
*
*        Changes:
*            SheathedActor
*				Will be attached to the given shoulder box assuming all conditions are met.
*
*        Return: Returns if the operation could be completed or not.
**********************************************************************************************************/
bool AVRPlayerController::SheatheItem(USceneComponent*  ShoulderBox, AActor* SheathedActor)
{
	if (ShoulderBox == nullptr || SheathedActor == nullptr)
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
	else
		return false;

	return true;
}
/**********************************************************************************************************
*	void ModifyHealth_Implementation(float Amount)
*		Purpose:    Potion Interface implementation. Causes the player to heal when using a health potion.
*					PotionItems will call this automatically if the conditions are correct.
*
*		Parameters:
*			float Amount
*				The amount to heal.
*
*			Changes:
*				CurrentHealth
*					Updated to reflect the new amount.
**********************************************************************************************************/
void AVRPlayerController::ModifyHealth_Implementation(float Amount)
{
	if (CurrentHealth < m_player->MaximumHealth)
	{
		CurrentHealth += Amount;

		if (m_healthBar != nullptr && m_player->MaximumHealth != 0)
			m_healthBar->SetFilledPercentage(CurrentHealth / m_player->MaximumHealth);
	}
}