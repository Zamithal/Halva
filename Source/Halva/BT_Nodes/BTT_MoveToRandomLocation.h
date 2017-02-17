// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveToRandomLocation.generated.h"

UENUM(BlueprintType)
enum class EWanderMethod : uint8
{
	FromSpecifiedPoint		UMETA(DisplayName="FromSpecifiedPoint"),
	FromCurrentPoint		UMETA(DisplayName="FromCurrentPoint"),
	FromSpawnPoint			UMETA(DisplayName="FromSpawnPoint")
};
/**********************************************************************************************************
*	Task: UBBT_MoveToRandomLocation
*
*	Overview:
*		This is a BT task node. Upon activation, the triggering pawn will move to a random point in a
*		circle from either the specified point, or, if not specified, its current location. The new
*		location is guaranteed to be outside the minimum wander radius but inside the maximum.
*
*	UProperties:
*		bool WaitForCompletion
*			Should the task wait until the move is completed or not before saying it is done.
*		EWanderMethod WanderMode
*			How should this actor calculate where to wander to?
*		float MinimumWanderRadius
*			The smallest radius the actor can move when this task activates.
*		float MaximumWanderRadius
*			The largest radius the actor can move when this task activates.
*		FVector OriginPoint
*			The location to stick around. Only applicable in FromSpecifiedPoint wander mode.
*
*	Data Members:
*		FVector m_wanderCenter
*			The current center to wander from.
**********************************************************************************************************/
UCLASS()
class HALVA_API UBTT_MoveToRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:

	// On creation.
	virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;

	// On execute.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void EventCompleted();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		bool WaitForCompletion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		EWanderMethod WanderMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		float MinimumWanderRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		float MaximumWanderRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		FVector OriginPoint;

private:
	FVector m_wanderCenter;
};
