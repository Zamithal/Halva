// © 2016 Skyler Swenson, some rights reserved.

#pragma once

#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_IsAttackIncoming.generated.h"

/**********************************************************************************************************
*	Task: UBTT_BaseEnemyAttack
*
*	Overview:
*		Checks to see if there is an incoming attack in the given attack list.
**********************************************************************************************************/
UCLASS()
class HALVA_API UBTD_IsAttackIncoming : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
	
public:


protected:

	virtual bool CalculateRawConditionValue ( UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory ) const override;
};
