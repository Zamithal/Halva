// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "ViveMovementComponent.h"
/**********************************************************************************************************
*	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
*		Purpose:	Consumes input given by the user. Moves the pawn dependent on the movement method
*					selected. After the movement has been calculated, gravity is added.
*		Parameters:
*			float DeltaTile
*				The time since the last tick.
*			enum ELevelTick TickType
*				The type of tick (Simulating, paused, etc.)
*			FActorComponentTickFunction *ThisTickFunction
*				Tick function that caused this to run.
**********************************************************************************************************/
void UViveMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
};


