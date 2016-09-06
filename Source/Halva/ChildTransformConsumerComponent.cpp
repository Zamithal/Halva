// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "ChildTransformConsumerComponent.h"
/**********************************************************************************************************
*	APlayerVive()
*		Purpose:	Default constructor.
**********************************************************************************************************/
UChildTransformConsumerComponent::UChildTransformConsumerComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	m_IgnoreBitField = 0;
}


// Called when the game starts
void UChildTransformConsumerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

/**********************************************************************************************************
*	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
*		Purpose:	Each tick, each child's local transforms are summed together. This transform is then
*					applied to the parent of this component and each child's transforms are 0'ed out. If
*					a specific aspect of the transform is set to be ignored, it will not be added to the
*					parent or 0'ed out.
*
*		Parameters:
*			float DeltaTile
*				The time since the last tick.
*			enum ELevelTick TickType
*				The type of tick (Simulating, paused, etc.)
*			FActorComponentTickFunction *ThisTickFunction
*				Tick function that caused this to run.
*
*		Changes:
*			All children below this will have no local transforms and the parent above this will have the
*			sum. This respects ignore rules.
**********************************************************************************************************/
void UChildTransformConsumerComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );


}
/**********************************************************************************************************
*	bool GetTransformIgnored(FTransformAspect Transform)
*		Purpose:	returns if the given transform is set to be ignored.
*
*		Parameters:
*			FTransformAspect Transform
*				The type of transform to check to see if it is being ignored.
*
*		Return:
*			Returns if the given transform is being ignored or not.
**********************************************************************************************************/
bool UChildTransformConsumerComponent::GetTransformIgnored(FTransformAspects Transform)
{
	if (m_IgnoreBitField & Transform)
		return true;
	else
		return false;
}
/**********************************************************************************************************
*	void SetTransformIgnored(FTransformAspect Transform)
*		Purpose:	Causes the given transform to be ignored. Transforms of that type will not be consumed.
*
*		Parameters:
*			FTransformAspect Transform
*				The type of transform to ignore.
*
*		Changes:
*			m_ignoreBitField
*				Bits will be updated to reflect change.
**********************************************************************************************************/
void UChildTransformConsumerComponent::SetTransformIgnored(FTransformAspects Transform)
{
	m_IgnoreBitField |= Transform;
}