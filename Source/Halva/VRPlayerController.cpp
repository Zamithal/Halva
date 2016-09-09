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

}

// Called when the game starts or when spawned
void AVRPlayerController::BeginPlay()
{
	

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
				playerPawn->FOVRestrictorWidget = FOVRestrictor;
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

void AVRPlayerController::BeginMoveLeft()
{
	m_LeftMovePressed = true;
	RestrictVision();
}

void AVRPlayerController::EndMoveLeft()
{
	m_LeftMovePressed = false;
	UnrestrictVision();
}

void AVRPlayerController::BeginMoveRight()
{
	m_RightMovePressed = true;
	RestrictVision();
}

void AVRPlayerController::EndMoveRight()
{
	m_RightMovePressed = false;
	UnrestrictVision();
}