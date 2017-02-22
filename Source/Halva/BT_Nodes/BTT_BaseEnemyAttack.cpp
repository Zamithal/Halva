// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "BaseEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTT_BaseEnemyAttack.h"

/**********************************************************************************************************
*    EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
*
*        Purpose:   If the owning AI does not inherit from BaseEnemyAIController, this will do nothing
*					and fail. When this node is activated, the pawn will attempt to play an attack
*					animation appropriate to hit the current enemy. If the pawn has no animation that can
*					hit the enemy from the current location. This node will do nothing instead and will
*					fail.
*
*        Parameters:
*           UBehaviorTreeComponent& OwnerComp
*				The behavior tree that fired this node.
*			uint8* NodeMemory
*				This is not well documented and will not be modified in any way.
*
*        Return:
*			Returns if an attack animation was played.
**********************************************************************************************************/
EBTNodeResult::Type UBTT_BaseEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseEnemyAIController* ai = Cast<ABaseEnemyAIController>(OwnerComp.GetAIOwner());

	if (ai == nullptr)
		return EBTNodeResult::Failed;

	AActor* currentEnemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CurrentEnemy"));

	if (currentEnemy == nullptr)
		return EBTNodeResult::Failed;

	ai->PerformAttack(currentEnemy);

	return EBTNodeResult::Succeeded;
}

