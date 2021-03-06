// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Strafe.generated.h"

/**********************************************************************************************************
*	enum: ERotationQuantity
*
*	Overview:
*		Specifies how the rotation quantity should be interpreted.
*
*	Options:
*		Degrees
*			The input is in degrees. The actor should strafe around the target until it has rotated the
*			given number of degrees.
*		Seconds
*			The input is in seconds. The actor should strafe around the target for the given number of 
*			seconds.
**********************************************************************************************************/
UENUM(BlueprintType)
enum class ERotationQuantity : uint8
{
	Degrees		UMETA(DisplayName = "Degrees"),
	Seconds		UMETA(DisplayName = "Seconds")
};
/**********************************************************************************************************
*	enum: ERotationDirection
*
*	Overview:
*		Specifies which way to rotate.
*
*	Options:
*		Clockwise
*			Rotates clockwise.
*		CounterClockwise
*			Rotates counter clockwise
*		Random
*			Rotates randomly clockwise or counter clockwise.
**********************************************************************************************************/
UENUM(BlueprintType)
enum class ERotationDirection : uint8
{
	Clockwise			UMETA(DisplayName = "Clockwise"),
	CounterClockwise	UMETA(DisplayName = "CounterClockwise"),
	Random				UMETA(DisplayName = "Random")
};
/**********************************************************************************************************
*	Task: UBTT_Strafe
*
*	Overview:
*		This is a BT task node. Upon activation, the triggering pawn will circle around the given target
*		for a configurable amount of time. This task will complete once the rotation has completed its
*		configured amount.
*
*	UProperties:
*		FName RotationTarget
*			The name of the blackboard key to rotate around. Must be an actor.
*		ERotationQuantity RotationMethod
*			Specifies how the completion event for the rotation event should be decided.
*		ERotationDirection Direction
*			Specifies the direction to rotate around the target.
*		float StrafeSpeed
*			The speed at which the actor strafes.
*		float RotationAmount
*			The amount to rotate around the target.
*		float MaximumRotationVariance
*			This is the largest amount that can be added or subtracted to the rotation for randomness.
*
*	Data Members:
*		float m_rotationRemaining
*			The amount of the rotation that has been completed.
*		AActor* m_rotationActor
*			The actor to rotate around.
*		FVector m_locationLastFrame
*			The location of the actor last frame.
**********************************************************************************************************/
UCLASS()
class HALVA_API UBTT_Strafe : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	// Constructor
	UBTT_Strafe();

	// Tick
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// On execute.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		ERotationQuantity RotationMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		ERotationDirection Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		float StrafeSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		float RotationAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		float MaximumRotationVariance;
	
private:

	float m_rotationRemaining;
	AActor* m_rotationActor;
	FVector m_locationLastFrame;
};
