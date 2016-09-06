// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "ViveMovementComponent.h"
/**********************************************************************************************************
*	UViveMovementComponent()
*		Purpose:	Default Constructor.
**********************************************************************************************************/
UViveMovementComponent::UViveMovementComponent()
{
	m_MovementSpeed = 100;
	m_GravityVector = FVector(0, 0, -1);
	m_GravityForce = 980;
}
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
*
*		Changes:
*			The pawn owning this component will be moved each frame dependent on user input.
**********************************************************************************************************/
void UViveMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool shouldUpdate = true;
	//Validate and check if update should be skipped.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
		shouldUpdate = false;

	//Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
	FVector PlayerInputVector = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * m_MovementSpeed;
	FVector GravityInput = m_GravityVector.GetClampedToMaxSize(1.0f) * DeltaTime * m_GravityForce;

	FVector FinalInput = PlayerInputVector + GravityInput;

	if (FinalInput.IsNearlyZero())
		shouldUpdate = false;

	if (shouldUpdate)
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(FinalInput, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(FinalInput, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
};
/**********************************************************************************************************
*	float GetMovementSpeed()
*		Purpose:	Getter.
**********************************************************************************************************/
float UViveMovementComponent::GetMovementSpeed()
{
	return m_MovementSpeed;
}
/**********************************************************************************************************
*	void SetMovementSpeed(float NewSpeed)
*		Purpose:	Setter.
**********************************************************************************************************/
void UViveMovementComponent::SetMovementSpeed(float NewSpeed)
{
	m_MovementSpeed = NewSpeed;
}
/**********************************************************************************************************
*	FVector GetGravityVector()
*		Purpose:	Getter.
**********************************************************************************************************/
FVector UViveMovementComponent::GetGravityVector()
{
	return m_GravityVector;
}
/**********************************************************************************************************
*	void SetGravityVector(FVector NewVector)
*		Purpose:	Setter.
**********************************************************************************************************/
void UViveMovementComponent::SetGravityVector(FVector NewVector)
{
	m_GravityVector = NewVector;
}
/**********************************************************************************************************
*	float GetGravityForce()
*		Purpose:	Getter.
**********************************************************************************************************/
float UViveMovementComponent::GetGravityForce()
{
	return m_GravityForce;
}
/**********************************************************************************************************
*	void SetGravityForce(float NewForce)
*		Purpose:	Setter.
**********************************************************************************************************/
void UViveMovementComponent::SetGravityForce(float NewForce)
{
	m_GravityForce = NewForce;
}