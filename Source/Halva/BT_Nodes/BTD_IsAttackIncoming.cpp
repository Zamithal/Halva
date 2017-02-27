// © 2016 Skyler Swenson, some rights reserved.

#include "Halva.h"
#include "AIIncomingAttackList.h"
#include "BTD_IsAttackIncoming.h"

/**********************************************************************************************************
*	CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
*
*	Overview:
*		Returns true if an attack is incoming.
**********************************************************************************************************/
bool UBTD_IsAttackIncoming::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return false;
}