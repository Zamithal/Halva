// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_BaseEnemyAttack.generated.h"

/**********************************************************************************************************
*	Task: UBTT_BaseEnemyAttack
*
*	Overview:
*		This is a BT task node. If the AIController does not inherit from BaseEnemyAIController, this node 
*		will fail. When this node is activated, the pawn will attempt to play an attack animation
*		appropriate to hit the current enemy. If the pawn has no animation that can hit the enemy from the
*		current location. This node will do nothing instead and will fail.
*
*	Methods:
*		AnimationCompleted(UBehaviorTreeComponent& OwnerComp)
*			Wrapper function that fires when the played animation is completed.
**********************************************************************************************************/
UCLASS()
class HALVA_API UBTT_BaseEnemyAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// On execute.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
		void AnimationCompleted(UBehaviorTreeComponent* OwnerComp);
	
private:
};
