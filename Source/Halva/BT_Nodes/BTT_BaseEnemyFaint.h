// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BaseEnemyFaint.generated.h"

/**********************************************************************************************************
*	Task: UBTT_BaseEnemyDodge
*
*	Overview:
*		This is a BT task node. If the AIController does not inherit from BaseEnemyAIController, this node
*		will fail. When this node is activated, the pawn will pick a fainting animation at random and
*		play it. If there are no assigned fainting animations, this node will fail.
*
*	Methods:
*		AnimationCompleted(UBehaviorTreeComponent& OwnerComp)
*			Wrapper function that fires when the played animation is completed.
**********************************************************************************************************/
UCLASS()
class HALVA_API UBTT_BaseEnemyFaint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
		void AnimationCompleted(UBehaviorTreeComponent* OwnerComp);

protected:
};
