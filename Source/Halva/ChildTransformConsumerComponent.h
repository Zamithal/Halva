// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "ChildTransformConsumerComponent.generated.h"
/**********************************************************************************************************
*	Class: ChildTransformConsumerComponent
*
*	Overview:
*		This component sums up the local transforms of each child. The sum is then transfered to the
*		parent of this component as its local transform. This class has several options for ignoring
*		specific parts of the transform, such as ignoring the Z axis when summing the children. All
*		local transforms applied to the parent that are not ignored will be lost when using this class.
*
*	Methods:
*		virtual void TickComponent(...) override;
*			Called each tick to consume child local transforms and transfer them to the parent.
*		bool GetTransformIgnored(FTransformAspects Transform)
*			Checks if the type of transform specified is currently ignored and returns true or false.
*		void SetTransformIgnored(FTransformAspects Transform)
*			Marks the given transform to be ignored. When consuming child transforms the given aspect will
*			not be consumed.
*		FVector ConsumePositionalTransform(USceneComponent * Child)
*			Consumes the child's transform along the axis and returns it.
*		FRotator ConsumeRotationalTransform(USceneComponent * Child)
*			Consumes the child's rotation and returns it.
*		FVector ConsumeScalarTransform(USceneComponent * Child)
*			Consumes the child's scale and returns it.
*
*	Data Members:
*		int m_IgnoreBitField
*			Bit field containing bools on what should be ignored when consuming transforms.
*			Use FTransformAspects to decode.
*		
**********************************************************************************************************/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALVA_API UChildTransformConsumerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChildTransformConsumerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	FVector ConsumePositionalTransform(USceneComponet * Parent, USceneComponent * Child);
	FRotator ConsumeRotationalTransform(USceneComponet * Parent, USceneComponent * Child);
	FVector ConsumeScalarTransform(USceneComponet * Parent, USceneComponent * Child);


	bool GetTransformIgnored(enum FTransformAspects Transform);
	void SetTransformIgnored(enum FTransformAspects Transform);

private:
	int m_IgnoreBitField;
};

enum FTransformAspects
{
	xVector = 2,
	yVector = 4,
	zVector = 8,
	allVectors = 14,
	xRotation = 16,
	yRotation = 32,
	zRotation = 64,
	allRoations = 112,
	xScale = 128,
	yScale = 256,
	zScale = 512,
	allScaling = 896,
	all = 1023
};