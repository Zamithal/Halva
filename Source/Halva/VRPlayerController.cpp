// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "Blueprint/UserWidget.h"
#include "VRPlayerController.h"

// Called when the game starts or when spawned
void AVRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// if the class was assigned.
	if (wFOVRestrictor)
	{
		// Attempt to create the widget
		FOVRestrictor = CreateWidget<UUserWidget>(this, wFOVRestrictor);

		if (FOVRestrictor)
		{
			// get the player pawn.
			AVRPawn * playerPawn = Cast<AVRPawn>(GetPawn());

			if (playerPawn)
				playerPawn->FOVRestrictorWidget = FOVRestrictor;
		}
	}
}