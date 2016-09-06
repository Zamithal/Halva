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
*	Mutators:
*		float MaxSpeed
*			-Get
*			-Set
*		FVector GravityVector
*			-Get
*			-Set
*		float GravityForce
*			-Get
*			-Set
*
*	Methods:
*		virtual void TickComponent(...) override
*			Handles input each tick to translate into motion.
*
*	Data Members:
*		float m_MovementSpeed
*			Top speed this actor should achieve in unreal units per second.
*		FVector m_GravityVector
*			Gravity vector is added every frame to the actor. This is a unit vector pointing in the
*			direction gravity pulls (Usually -Z).
*		float m_GravityForce
*			The force gravity exerts in unrealUnits per second.
**********************************************************************************************************/
UCLASS()
class HALVA_API UViveMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UViveMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	float GetMovementSpeed();
	void SetMovementSpeed(float NewSpeed);
	FVector GetGravityVector();
	void SetGravityVector(FVector NewDirection);
	float GetGravityForce();
	void SetGravityForce(float NewForce);

private:
	float m_MovementSpeed;
	FVector m_GravityVector;
	float m_GravityForce;
};
