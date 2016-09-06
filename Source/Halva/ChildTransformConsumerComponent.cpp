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

	//Immediate Parent Component
	USceneComponent * parent = this->GetAttachParent();
	// List of all child components.
	TArray<USceneComponent *> childComponents = this->GetAttachChildren();

	// If there is at least one child and at leas one parent.
	if (parent != nullptr && childComponents.Num() > 0)
	{
		// Final transform
		FTransform finalTransform = FTransform();

		FTransform currentTransform = FTransform();

		for (int i = 0; i < childComponents.Num(); i++)
		{
			if (childComponents[i] != nullptr)
			{
				currentTransform.SetLocation(ConsumePositionalTransform(childComponents[i]));
				currentTransform.SetRotation(ConsumeRotationalTransform(childComponents[i]).Quaternion());
				currentTransform.SetScale3D(ConsumeScalarTransform(childComponents[i]));

				// Accumulate this transform into the final transformation.
				finalTransform.Accumulate(currentTransform, ScalarOne);
			}
		}

		// Normalize rotator.
		FQuat normalizedFinalRot = finalTransform.GetRotation();
		normalizedFinalRot.Normalize();
		finalTransform.SetRotation(normalizedFinalRot);
	}

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
/**********************************************************************************************************
*	FVector ConsumePositionalTransform(USceneComponet * Parent, USceneComponent * Child)
*		Purpose:	Gets the positional transform of the child respecting ignored axis. The transform is
*					then set to 0 for all non-ignored axis.
*
*		Parameters:
*			USceneComponent * Child
*				The child to consume transforms from.
*
*		Changes:
*			USceneComponent * Child
*				This child's local transforms will become 0 for non-ignored axis.
*
*		Return:
*			Returns the child's axis transform before consumption.
**********************************************************************************************************/
FVector UChildTransformConsumerComponent::ConsumePositionalTransform(USceneComponet * Parent, USceneComponent * Child)
{
	if (Child == nullptr)
		return FVector();

	FVector consumedTransform = Child->GetComponentLocation();

	if (consumedTransform == FVector(0, 0, 0))
		return consumedTransform;

	if (GetTransformIgnored(FTransformAspects::xVector))
		consumedTransform.X = 0;

	if (GetTransformIgnored(FTransformAspects::yVector))
		consumedTransform.Y = 0;

	if (GetTransformIgnored(FTransformAspects::zVector))
		consumedTransform.Z = 0;

	return consumedTransform;
}
/**********************************************************************************************************
*	FRotator ConsumeRotationalTransform(USceneComponet * Parent, USceneComponent * Child)
*		Purpose:	Gets the positional transform of the child respecting ignored rotations. The transform 
*					is then set to 0 for all non-ignored rotations.
*
*		Parameters:
*			USceneComponent * Child
*				The child to consume transforms from.
*
*		Changes:
*			USceneComponent * Child
*				This child's local transforms will become 0 for non-ignored rotations.
*
*		Return:
*			Returns the child's rotational transforms before consumption.
**********************************************************************************************************/
FRotator UChildTransformConsumerComponent::ConsumeRotationalTransform(USceneComponet * Parent, USceneComponent * Child)
{
	if (Child == nullptr)
		return FRotator();

	FRotator consumedTransform = Child->GetComponentRotation();

	if (GetTransformIgnored(FTransformAspects::xRotation))
		consumedTransform.Roll = 0;

	if (GetTransformIgnored(FTransformAspects::yRotation))
		consumedTransform.Pitch = 0;

	if (GetTransformIgnored(FTransformAspects::zRotation))
		consumedTransform.Yaw = 0;

	return consumedTransform;
}
/**********************************************************************************************************
*	FVector ConsumeScalarTransform(USceneComponet * Parent, USceneComponent * Child)
*		Purpose:	Gets the positional transform of the child respecting ignored scalings. The transform
*					is then set to 0 for all non-ignored scalings.
*
*		Parameters:
*			USceneComponent * Child
*				The child to consume transforms from.
*
*		Changes:
*			USceneComponent * Child
*				This child's local transforms will become 1 for non-ignored scalings.
*
*		Return:
*			Returns the child's scale transform before consumption.
**********************************************************************************************************/
FVector UChildTransformConsumerComponent::ConsumeScalarTransform(USceneComponent * Child)
{
	if (Child == nullptr)
		return FVector(1, 1, 1);

	FVector newChildTransform = Child->GetComponentScale();
	FVector newParentTransform = newChildTransform;

	if (GetTransformIgnored(FTransformAspects::xScale))
		newParentTransform.X = 1;
	else
		newChildTransform.X = 1;

	if (GetTransformIgnored(FTransformAspects::yScale))
		newParentTransform.Y = 1;
	else
		newChildTransform.Y = 1;


	if (GetTransformIgnored(FTransformAspects::zScale))
		newParentTransform.Z = 1;
	else
		newChildTransform.Z = 1;

	return newParentTransform;
}