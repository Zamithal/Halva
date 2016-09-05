// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "ViveMovementComponent.generated.h"

/**********************************************************************************************************
*	Class: ViveMovementComponent
*
*	Overview:
*		This is a pawn movement component used for input from the HTC Vive. The class allows the pawn it
*		is attached to to navigate using the motion controllers. Additionally, basic constants like
*		gravity are applied to the pawn as well.
*
*	Methods:
*		virtual void TickComponent(...) override
*			Handles input each tick to translate into motion.
*
*	Data Members:
**********************************************************************************************************/
UCLASS()
class HALVA_API UViveMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	
};
