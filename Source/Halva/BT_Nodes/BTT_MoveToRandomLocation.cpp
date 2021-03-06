// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "AIController.h"
#include "Tasks/AITask_MoveTo.h"
#include "BTT_MoveToRandomLocation.h"


/**********************************************************************************************************
*    void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
*        Purpose:   Marks the current location as the center of wandering if wander mode is set to spawn.
*
*        Parameters:
*           UBehaviorTreeComponent& OwnerComp
*				The behavior tree this node belongs to.
**********************************************************************************************************/
void UBTT_MoveToRandomLocation::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
	
	switch (WanderMode)
	{
	case EWanderMethod::FromSpecifiedPoint:
		m_wanderCenter = OriginPoint;
		break;
	case EWanderMethod::FromCurrentPoint:
		break;
	case EWanderMethod::FromSpawnPoint:
		m_wanderCenter = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
		break;
	default:
		break;
	}	
}
/**********************************************************************************************************
*    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
*
*        Purpose:   Moves the actor to a random location in the specified radius. The center of the radius
*					can be different depending on the wander mode.
*
*        Parameters:
*           UBehaviorTreeComponent& OwnerComp
*				The behavior tree that fired this node.
*			uint8* NodeMemory
*				This is not well documented and will not be modified in any way.
*
*        Return:
*			Returns if the actor successfully moved or not.
**********************************************************************************************************/
EBTNodeResult::Type UBTT_MoveToRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	FVector randomVector = FMath::VRand();

	randomVector.Z = 0;

	randomVector.Normalize();

	randomVector *= FMath::RandRange(MinimumWanderRadius, MaximumWanderRadius);

	UAITask_MoveTo* currentOperation = nullptr;

	switch (WanderMode)
	{
	case EWanderMethod::FromSpecifiedPoint:
		OwnerComp.GetAIOwner()->MoveToLocation(m_wanderCenter + randomVector);
		break;
	case EWanderMethod::FromCurrentPoint:
		m_wanderCenter = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
		OwnerComp.GetAIOwner()->MoveToLocation(m_wanderCenter + randomVector);
		break;
	case EWanderMethod::FromSpawnPoint:
		OwnerComp.GetAIOwner()->MoveToLocation(m_wanderCenter + randomVector);
		break;
	default:
		break;
	}

	if (WaitForCompletion)
		return EBTNodeResult::InProgress;
	else
		return EBTNodeResult::Succeeded;
}

void UBTT_MoveToRandomLocation::EventCompleted()
{
	int x = 4;
	x += 4;
}
